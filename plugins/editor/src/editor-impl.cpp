#include <QDebug>
#include <QPainter>
#include <QTimer>
#include "editor-impl.h"
#include "syntax/highlighting.h"
#include "syntax/definition.h"
#include "syntax/theme.h"
#include "settings.h"
#include "editor-margin.h"
#include "includes/file-handler.h"

EditorImpl::EditorImpl(const QString& fname, const syntax::DefinitionPtr& def):
    m_fileName(fname),
    m_definition(def),
    m_syntax(def ? new syntax::Highlighting(document(), def) : nullptr)
{
    setFont(Settings::font());
    setWordWrapMode(Settings::wordWrap() ? QTextOption::WordWrap : QTextOption::NoWrap);

    QTextOption opt = document()->defaultTextOption();
    if (Settings::showWhite())
        opt.setFlags(opt.flags() | QTextOption::ShowTabsAndSpaces);
    if (Settings::showEndl())
        opt.setFlags(opt.flags() | QTextOption::ShowLineAndParagraphSeparators);
    document()->setDefaultTextOption(opt);

    setTabStopWidth(4 * fontMetrics().width(' '));

    if (Settings::showLineNumbers()){
        m_margin = new EditorMargin(this);
        m_margin->setFont(font());
    }

    connect(this, &EditorImpl::modificationChanged, [this](bool modified){
        qnoto::FileHandler::instance().modified(fileName(), modified);
    });
}

EditorImpl::~EditorImpl()
{
}

void EditorImpl::init()
{
    syntax::ThemePtr theme = syntax::ThemePtr::create();
    if (theme->load(Settings::theme())){
        if (m_syntax)
            m_syntax->setTheme(theme);

        auto pal = palette();
        pal.setColor(QPalette::Base, theme->backgroud());
        pal.setColor(QPalette::Text, theme->color());
        setPalette(pal);
        m_theme = theme;
    }
}

const syntax::ThemePtr& EditorImpl::theme() const
{
    return m_theme;
}

void EditorImpl::load(const QString& text)
{
    setEnabled(false);
    setUndoRedoEnabled(false);
    setPlainText(text);
    setUndoRedoEnabled(true);
    setEnabled(true);
}

const QString& EditorImpl::fileName() const
{
    return m_fileName;
}

void EditorImpl::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    if (m_margin){
        m_margin->setGeometry(QRect(cr.left(), cr.top(), m_margin->preferedWidth(), cr.height()));
        setViewportMargins(m_margin->preferedWidth(), 0, 0, 0);
    }
}

void EditorImpl::startFind(const QString& text)
{
    m_finder.pattern = text;
    m_finder.isSet = true;
    repaint();
}

void EditorImpl::unmark()
{
    m_finder.isSet = false;
    repaint();
}

//void EditorImpl::paintEvent(QPaintEvent* e)
//{
//    QPlainTextEdit::paintEvent(e);
//    if (m_finder.isSet){
//        //QPainter paint(viewport());
//        QTextBlock block = firstVisibleBlock();
//        while(block.isValid()){
//            if (!block.isVisible())
//                continue;
//            //int index = m_finder.find()
//            //if (block.text()
//            block = block.next();
//        }
//    }
//}
