#pragma once
#include <QSyntaxHighlighter>
#include "definition.h"

class QTextDocument;

namespace syntax {

FORWARD(Theme)
struct State;

class Highlighting: public QSyntaxHighlighter
{
public:
    Highlighting(QTextDocument* doc, const DefinitionPtr& def, bool markWhite = true);
    void setTheme(const ThemePtr& theme);

protected:
    void highlightBlock(const QString &text) override;
    void highlightLine(const QString& text, const QSharedPointer<State>& state);
    void applyFormat(const QString& frm, int from, int to);
    void contextChanged(const ContextPtr& oc, int& begin, int end);
private:
    DefinitionPtr m_definition;
    ThemePtr      m_theme;
    bool          m_markWhite;
};

}
