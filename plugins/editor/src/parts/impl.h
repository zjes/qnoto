#pragma once
#include <QPlainTextEdit>
#include <QScopedPointer>
#include "includes/plugin.h"

namespace syntax
{
FORWARD(Definition)
FORWARD(Highlighting)
FORWARD(Theme)
}
class EditorMargin;

class Finder
{
public:
    QString pattern;
    bool isSet = false;
    int find(const QString& text, QString& width, int offset = 0);
};


class EditorImpl: public QPlainTextEdit
{
    Q_OBJECT
friend class EditorMargin;
public:
    EditorImpl(const QString& fname, const syntax::DefinitionPtr& def);
    ~EditorImpl();

    void init();
    const syntax::ThemePtr& theme() const;
    void load(const QString& text);
    QString text() const;
    const QString& fileName() const;
    Finder& finder();
public slots:
    void startFind(const QString& text);
    void unmark();
    void doFind();
signals:
    void showFind();
    void escape();
    void findAvail(bool avail);
protected:
    void resizeEvent(QResizeEvent*e ) override;
    void paintEvent(QPaintEvent *e) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool event(QEvent* ev) override;
private:
    QString                              m_fileName;
    syntax::DefinitionPtr                m_definition;
    QScopedPointer<syntax::Highlighting> m_syntax;
    EditorMargin*                        m_margin = nullptr;
    syntax::ThemePtr                     m_theme;
    Finder                               m_finder;
    bool                                 m_needPaintSearch = false;
};
