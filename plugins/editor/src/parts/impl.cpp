#include <QDebug>
#include <QTime>
#include <QPainter>
#include <QTimer>
#include <QKeyEvent>
#include <QScrollBar>
#include <QStyle>
#include <QToolTip>

#include "impl.h"
#include "margin.h"
#include "scroll.h"

#include "syntax/highlighting.h"
#include "syntax/definition.h"
#include "syntax/theme.h"
#include "syntax/userdata.h"
#include "settings.h"
#include "includes/file-handler.h"

EditorImpl::EditorImpl(const QString& fname, const syntax::DefinitionPtr& def):
    m_fileName(fname),
    m_definition(def),
    m_syntax(def ? new syntax::Highlighting(document(), def, Settings::showWhite()) : nullptr)
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
        emit qnoto::FileHandler::instance().modified(fileName(), modified);
    });

    setVerticalScrollBar(new Scroll);
}

EditorImpl::~EditorImpl()
{
}

void EditorImpl::init()
{
    syntax::ThemePtr theme = syntax::ThemePtr::create();
    if (!theme->load(Settings::theme()))
        return;

    if (m_syntax)
        m_syntax->setTheme(theme);

    auto pal = palette();
    pal.setColor(QPalette::Base, theme->backgroud());
    pal.setColor(QPalette::Text, theme->color());
    setPalette(pal);
    m_theme = theme;
}

const syntax::ThemePtr& EditorImpl::theme() const
{
    return m_theme;
}

void EditorImpl::load(const QString& text)
{
    setPlainText(text);
}

QString EditorImpl::text() const
{
    return toPlainText();
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

void EditorImpl::paintEvent(QPaintEvent* event)
{
    QPainter paint(viewport());

    QTextBlock block = firstVisibleBlock();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            if (m_syntax->hasUserData(block))
                m_syntax->paintBlock(block, paint, blockBoundingGeometry(block).translated(contentOffset()).toRect());
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
    }

    QPlainTextEdit::paintEvent(event);
}

bool EditorImpl::event(QEvent* ev)
{
    if (ev->type() != QEvent::ToolTip)
        return QPlainTextEdit::event(ev);

    QHelpEvent *te = static_cast<QHelpEvent*>(ev);
    QTextCursor cursor = cursorForPosition(te->pos());
    QTextBlock block = cursor.block();
    if (!block.isValid())
        return QPlainTextEdit::event(ev);

    syntax::Folding* folding = syntax::folding(block);
    if (!folding)
        return QPlainTextEdit::event(ev);

    if (folding->closed){
        QPoint mpos = viewport()->mapFromParent(te->pos());
        qreal left = block.layout()->lineAt(0).cursorToX(folding->offset);
        if (mpos.x() > left && mpos.x() < left+40){
            QStringList text = {block.text()};
            for(auto blk = block.next(); blk.isValid(); blk = blk.next()){
                text << blk.text();

                if (folding->checkEndBlock(blk))
                    break;
            }

            QRectF br = blockBoundingGeometry(block).translated(contentOffset());
            QRectF pr = QRectF(left, br.top(), 40, br.height());
            QFont fnt = font();
            fnt.setPointSize(static_cast<int>(ceil(font().pointSize()*0.8)));
            QToolTip::setFont(fnt);
            QToolTip::showText(te->globalPos(), text.join("\n"), this, pr.toRect());
        } else {
            QToolTip::hideText();
            ev->ignore();
        }
    }

    return true;
}

void EditorImpl::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        emit escape();
    QPlainTextEdit::keyPressEvent(event);
}

void EditorImpl::startFind(const QString& text)
{
    m_finder.pattern = text;
    m_finder.isSet = true;

    if (text.isEmpty()){
        dynamic_cast<Scroll*>(verticalScrollBar())->setPosses({}, blockCount());
        unmark();
        return;
    }

    QString found;
    std::map<int, QString> idxs;
    auto* doc = document();
    QTextFrameFormat frm;
    frm.setBorder(1);

    QList<int> posses;
    posses.reserve(blockCount());

    for (QTextBlock block = doc->begin(); block != doc->end(); block = block.next()){
        int index = -1;
        while((index = m_finder.find(block.text(), found, index+1)) != -1){
            idxs.emplace(index, found);
        }
        if (!idxs.empty()){
            m_syntax->setFound(block, idxs);
            idxs.clear();
            posses.append(block.blockNumber());
        } else {
            m_syntax->clearFound(block);
        }
    }

    dynamic_cast<Scroll*>(verticalScrollBar())->setPosses(posses, blockCount());
    qobject_cast<QPlainTextDocumentLayout*>(document()->documentLayout())->requestUpdate();
}

void EditorImpl::doFind()
{
    QString found;
    auto cur = textCursor();
    for(QTextBlock block = cur.block(); block.isValid(); block = block.next()){
        int index = block == cur.block() ? cur.positionInBlock() : -1;
        while((index = m_finder.find(block.text(), found, index+1)) != -1){
            if (block.position()+index <= cur.position())
                continue;
            cur.setPosition(block.position()+index);
            cur.selectionStart();
            cur.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor, found.length());
            cur.selectionEnd();
            setTextCursor(cur);
            return;
        }
    }
}

void EditorImpl::unmark()
{
    m_finder.isSet = false;
    auto* doc = document();
    for (QTextBlock block = doc->begin(); block != doc->end(); block = block.next()){
        m_syntax->clearFound(block);
    }
    m_needPaintSearch = true;
    qobject_cast<QPlainTextDocumentLayout*>(document()->documentLayout())->requestUpdate();
}

//--------------------------------------------------------------------------------------------------

int Finder::find(const QString& text, QString& found, int offset)
{
    found = pattern;
    return text.indexOf(pattern, offset, Qt::CaseInsensitive);
}
