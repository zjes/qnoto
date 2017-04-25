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

struct UserData: public QTextBlockUserData
{
    QSharedPointer<State>  state;
    std::map<int, QString> markFound;
};

Highlighting::Highlighting(QTextDocument* doc, const DefinitionPtr& def, bool markWhite):
    QSyntaxHighlighter(doc),
    m_definition(def),
    m_markWhite(markWhite)
{}

void Highlighting::setTheme(const ThemePtr& theme)
{
    m_theme = theme;
}


void Highlighting::highlightBlock(const QString& text)
{
    if (m_definition){
        QSharedPointer<State> prevState;
        if (currentBlock().position() > 0) {
            UserData* prevData = dynamic_cast<UserData*>(currentBlock().previous().userData());
            if (prevData)
                prevState = prevData->state;
        }

        UserData* data = dynamic_cast<UserData*>(currentBlockUserData());
        if (!data) {
            data = new UserData;
            setCurrentBlockUserData(data);
        }
        if (!data->state){
            data->state = QSharedPointer<State>::create();
        } else {
            data->state->contexts.clear();
        }
        if (prevState && !prevState->contexts.isEmpty())
            data->state->contexts = prevState->contexts;
        else
            data->state->contexts.push(m_definition->context());

        highlightLine(text, data->state);
    }

    if (m_markWhite){
        QTextCharFormat format;
        if (fetchFormat("dsWhitespace", format)) {
            int offset = 0;
            int from = 0;
            bool white = false;
            while (offset < text.length()) {
                if (text[offset].isSpace() && !white) {
                    white = true;
                    from = offset;
                }
                else if (!text[offset].isSpace() && white) {
                    white = false;
                    applyFormat(format, from, offset - from);
                }
                ++offset;
            }
            if (white && from < offset)
                applyFormat(format, from, offset - from);
        }
    }

    QTextCharFormat format;
    if (fetchFormat("dsFoundMark", format)) {
        UserData* data = dynamic_cast<UserData*>(currentBlockUserData());
        for (const auto& mark : data->markFound) {
            applyFormat(format, mark.first, mark.second.length());
        }
    }
}

void Highlighting::highlightLine(const QString& text, const QSharedPointer<State>& state)
{
    ContextPtr ctx = state->contexts.top();

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

            QString nctx = rule->context();
            if(nctx.isEmpty())
                continue;

            lookAhead = rule->lookAhead();
            if (lookAhead){
                ctx = state->switchState(m_definition, nctx);
            } else {
                if (rule->context() != "#stay"){
                    applyFormat(rule->attribute(), beginOffset, newOffset-beginOffset);
                    ctx = state->switchState(m_definition, nctx);
                } else {
                    applyFormat(rule->attribute(), offset, newOffset-offset);
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
        applyFormat(ctx->attribute(), beginOffset, text.size()-beginOffset);

    if (ctx){
        QString nctx = ctx->lineEndContext();
        if (!nctx.isEmpty())
            ctx = state->switchState(m_definition, nctx);
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

void Highlighting::applyFormat(const QString& frm, int from, int length)
{
    static QTextCharFormat format;
    if (fetchFormat(frm, format)) {
        applyFormat(format, from, length);
    }
}

void Highlighting::applyFormat(const QTextCharFormat& format, int from, int length)
{
    setFormat(from, length, format);
}


void Highlighting::contextChanged(const ContextPtr& oc, int& begin, int end)
{
    applyFormat(oc->attribute(), begin, end);
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
    UserData* data = dynamic_cast<UserData*>(block.userData());
    if (!data)
        return false;
    if (data->markFound.empty())
        return false;

    auto line = block.layout()->lineAt(0);
    painter.setPen(QColor("#aeaeae"));
    painter.setBrush(QColor("#dddd99"));
    for (const auto& mark : data->markFound) {
        qreal left = line.cursorToX(mark.first);
        qreal right = line.cursorToX(mark.first+mark.second.length());
        QRectF pr = QRectF(bounding.left()+left, bounding.top(), right - left, bounding.height());
        painter.drawRoundedRect(pr, 3, 3);
    }
    return true;
}

}
