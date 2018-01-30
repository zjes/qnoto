#pragma once
#include <QMutex>
#include "syntaxhighlighter.h"
#include "definition.h"

class QTextDocument;

namespace syntax {

FORWARD(Theme)
struct UserData;

class Highlighting: public SyntaxHighlighter
{
public:
    Highlighting(QTextDocument* doc, const DefinitionPtr& def, bool markWhite = true);
    void setTheme(const ThemePtr& theme);

    void setFound(QTextBlock& block, const std::map<int, QString>& idx);
    void clearFound(QTextBlock& block);
    bool hasUserData(const QTextBlock& block);
    bool paintBlock(const QTextBlock& block, QPainter& painter, const QRect& bounding);
protected:
    void highlightBlock(TextBlock &block) override;
    void highlightLine(TextBlock& block, UserData* data);
    bool fetchFormat(const QString& frm, QTextCharFormat& format);
    void applyFormat(TextBlock& block, const QString& format, int from, int to);
    void applyFormat(TextBlock& block, const QTextCharFormat& format, int from, int length);
    void contextChanged(TextBlock& block, const ContextPtr& oc, int& begin, int end);
private:
    DefinitionPtr m_definition;
    ThemePtr      m_theme;
    bool          m_markWhite;
    QMutex        m_mutex;
};

}
