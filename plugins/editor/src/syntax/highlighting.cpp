#include <QDebug>
#include <QStack>
#include <QCoreApplication>
#include <QTextDocument>
#include <QPainter>
#include <QPlainTextDocumentLayout>
#include "highlighting.h"
#include "context.h"
#include "rules/rule.h"
#include "item-data.h"
#include "theme.h"
#include "userdata.h"

namespace syntax {

struct State
{
    QStack<ContextPtr> contexts;

    ContextPtr switchState(const DefinitionPtr& def, const QString& ctx)
    {
        if (ctx.startsWith("#pop")){
            int offset = 0;
            while(offset < ctx.length() && ctx.midRef(offset, 4).compare(QString("#pop")) == 0){
                if (!contexts.isEmpty())
                    contexts.pop();
                offset += 4;
            }
            if (contexts.isEmpty()){
                contexts.push(def->context());
            }
        } else if (!ctx.isEmpty() && ctx != "#stay"){
            auto nctx = def->context(ctx);
            if (!nctx){
                qWarning() << "Unknown context" << ctx;
            }
            contexts.push(nctx);
        }
        return contexts.top();
    }
};

Highlighting::Highlighting(QTextDocument* doc, const DefinitionPtr& def, bool markWhite):
    SyntaxHighlighter(doc),
    m_definition(def),
    m_markWhite(markWhite)
{}

void Highlighting::setTheme(const ThemePtr& theme)
{
    m_theme = theme;
}


void Highlighting::highlightBlock(TextBlock& block)
{
    QMutexLocker locker(&m_mutex);
    if (!block.block.isValid() || !block.block.isVisible())
        return;

    if (m_definition && !m_definition->isEmpty()){
        QSharedPointer<State> prevState;
        if (block.block.position() > 0) {
            UserData* prevData = dynamic_cast<UserData*>(block.block.previous().userData());
            if (prevData)
                prevState = prevData->state;
        }

        UserData* data = dynamic_cast<UserData*>(block.block.userData());
        if (!data) {
            data = new UserData;
            block.block.setUserData(data);
        }
        if (!data->state){
            data->state = QSharedPointer<State>::create();
        } else {
            data->state->contexts.clear();
        }
        if (prevState && !prevState->contexts.isEmpty() && prevState->contexts.last()->lineEndContext() == "#stay"){
            data->state->contexts.push(prevState->contexts.last());
        } else {
            data->state->contexts.push(m_definition->context());
        }

        highlightLine(block, data);
        UserData* nextData = dynamic_cast<UserData*>(block.block.next().userData());
        block.stateChanged = nextData && data->state->contexts.last() != nextData->state->contexts.first();
    }

    if (m_markWhite){
        QTextCharFormat format;
        if (fetchFormat("dsWhitespace", format)) {
            int offset = 0;
            int from = 0;
            bool white = false;
            QString text = block.block.text();
            while (offset < text.length()) {
                if (text[offset].isSpace() && !white) {
                    white = true;
                    from = offset;
                }
                else if (!text[offset].isSpace() && white) {
                    white = false;
                    applyFormat(block, format, from, offset - from);
                }
                ++offset;
            }
            if (white && from < offset)
                applyFormat(block, format, from, offset - from);
        }
    }
}

int nextId()
{
    static int id = 0;
    return ++id;
}

void Highlighting::highlightLine(TextBlock& block, UserData* data)
{
    ContextPtr ctx = data->state->contexts.top();

    QString text = block.block.text();
    int offset = 0;
    while(offset < text.length() && text[offset].isSpace())
        ++offset;

    int beginOffset = offset;
    int newOffset = offset;

    while(offset < text.length()){
        bool lookAhead = false;
        for(const auto& rule: ctx->rules()){
            newOffset = rule->match(text, offset);
            if (newOffset <= offset)
                continue;

            if (!rule->beginRegion().isEmpty()){
                data->folding.offset = newOffset;
                data->folding.type = Folding::Begin;
                if (!data->folding.id)
                    data->folding.id = nextId();
            }

            if (!rule->endRegion().isEmpty()){
                if (data->folding){
                    data->folding.type = Folding::None;
                } else {
                    data->folding.offset = newOffset;
                    data->folding.type = Folding::End;

                    int ocount = 1;
                    for(QTextBlock blk = block.block.previous(); blk.isValid(); blk = blk.previous()){
                        Folding* prev = folding(blk, true);
                        if (!prev)
                            continue;

                        if (prev->type == Folding::End){
                            ++ocount;
                            continue;
                        }

                        if (prev->type == Folding::Begin){
                            --ocount;
                            if (!ocount){
                                data->folding.id = prev->id;
                                break;
                            }
                        }
                    }
                }
            }

            QString nctx = rule->context();
            if(nctx.isEmpty())
                continue;

            lookAhead = rule->lookAhead();
            if (lookAhead){
                ctx = data->state->switchState(m_definition, nctx);
            } else {
                if (rule->context() != "#stay"){
                    applyFormat(block, rule->attribute(), beginOffset, newOffset-beginOffset);
                    ctx = data->state->switchState(m_definition, nctx);
                } else {
                    applyFormat(block, rule->attribute(), offset, newOffset-offset);
                }
                beginOffset = newOffset;
            }
            break;
        }
        if (lookAhead)
            continue;

        if (newOffset <= offset)
            newOffset = offset + 1;

        offset = newOffset;
    }
    if (beginOffset < offset)
        applyFormat(block, ctx->attribute(), beginOffset, text.size()-beginOffset);

    if (ctx){
        QString nctx = ctx->lineEndContext();
        if (!nctx.isEmpty())
            ctx = data->state->switchState(m_definition, nctx);
    }
}

bool Highlighting::fetchFormat(const QString& frm, QTextCharFormat& format)
{
    ItemDataPtr item = m_definition->itemData(frm);
    if (item && m_theme) {
        m_theme->format(format, item->name(), item->style());
        return true;
    }
    return false;
}

void Highlighting::applyFormat(TextBlock& block, const QString& format, int from, int length)
{
    QTextCharFormat f;
    if (fetchFormat(format, f)) {
        applyFormat(block, f, from, length);
    }
}

void Highlighting::applyFormat(TextBlock& block, const QTextCharFormat& format, int from, int length)
{
    block.setFormat(from, length, format);
}


void Highlighting::contextChanged(TextBlock& block, const ContextPtr& oc, int& begin, int end)
{
    applyFormat(block, oc->attribute(), begin, end);
    begin = end;
}

void Highlighting::setFound(QTextBlock& block, const std::map<int, QString>& idx)
{
    UserData* data = dynamic_cast<UserData*>(block.userData());
    if (!data) {
        data = new UserData;
        block.setUserData(data);
    }
    data->markFound = idx;
    //rehighlightBlock(block);
}

void Highlighting::clearFound(QTextBlock& block)
{
    UserData* data = static_cast<UserData*>(block.userData());
    if (data){
        bool rehigh = !data->markFound.empty();
        data->markFound.clear();
        if (rehigh){
            //rehighlightBlock(block);
        }
    }
}

bool Highlighting::paintBlock(const QTextBlock& block, QPainter& painter, const QRect& bounding)
{
    UserData* data = syntax::userData(block);
    auto line = block.layout()->lineAt(0);

    painter.setRenderHints(QPainter::Antialiasing|QPainter::SmoothPixmapTransform);

    QTextCharFormat format;
    fetchFormat("dsFoundMark", format);
    painter.setPen(format.foreground().color());
    painter.setBrush(format.background());
    int radius = format.intProperty(QTextFormat::UserProperty+1);

    for (const auto& mark : data->markFound) {
        qreal left = line.cursorToX(mark.first);
        qreal right = line.cursorToX(mark.first+mark.second.length());
        QRectF pr = QRectF(bounding.left()+left, bounding.top(), right - left, bounding.height());
        painter.drawRoundedRect(pr, radius, radius);
    }

    fetchFormat("dsNormal", format);
    painter.setPen(format.foreground().color());
    painter.setBrush(format.background());

    if (data->folding.type == Folding::Begin && data->folding.closed){
        qreal left = line.cursorToX(data->folding.offset);
        QRectF pr = QRectF(bounding.left()+left, bounding.top(), 40, bounding.height());
        painter.drawRoundedRect(pr, 4, 4);
        painter.drawText(pr, Qt::AlignCenter, "...");
    }
    return true;
}

bool Highlighting::hasUserData(const QTextBlock& block)
{
    UserData* data = dynamic_cast<UserData*>(block.userData());
    if (data && (!data->markFound.empty() || (data->folding.offset && data->folding.closed)))
        return true;
    return false;
}

}
