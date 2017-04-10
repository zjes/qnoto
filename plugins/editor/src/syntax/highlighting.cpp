#include <QDebug>
#include <QStack>
#include <QCoreApplication>
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
    QSharedPointer<State> state;
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


void Highlighting::highlightBlock(const QString &text)
{
    if (!m_definition)
        return;

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
    QCoreApplication::processEvents();
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
    if (m_markWhite){
        offset = 0;
        int from = 0;
        bool white = false;
        while(offset < text.length()){
            if (text[offset].isSpace() && !white){
                white = true;
                from = offset;
            } else if (!text[offset].isSpace() && white){
                white = false;
                applyFormat("dsWhitespace", from, offset-from);
            }
            ++offset;
        }
        if (white && from < offset)
            applyFormat("dsWhitespace", from, offset-from);
    }
}

void Highlighting::applyFormat(const QString& frm, int from, int length)
{
    ItemDataPtr item = m_definition->itemData(frm);

    if (item && m_theme){
        QTextCharFormat format;
        m_theme->format(format, item->name(), item->style());
        setFormat(from, length, format);
    }
}

void Highlighting::contextChanged(const ContextPtr& oc, int& begin, int end)
{
    applyFormat(oc->attribute(), begin, end);
    begin = end;
}

}
