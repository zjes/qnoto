#include <QDebug>
#include <QPainter>
#include <QTextBlock>
#include <QTimer>
#include <QStyle>
#include <QStyleOptionViewItem>

#include "margin.h"
#include "impl.h"

#include "syntax/theme.h"
#include "syntax/userdata.h"

#define FOLD_WIDTH 19

EditorMargin::EditorMargin(EditorImpl* editor):
    QWidget(editor),
    m_editor(editor)
{
    setMouseTracking(true);

    connect(m_editor, &EditorImpl::blockCountChanged, [this](int){
        repaint();
    });

    connect(m_editor, &EditorImpl::updateRequest, [this](const QRect& rect, int dy){
        if (dy)
            scroll(0, dy);
        else
            update(0, rect.y(), width(), rect.height());
    });
}

int EditorMargin::preferedWidth() const
{
    return 4 + fontMetrics().width('0') * QString::number(m_editor->blockCount()+1).length() + 4 + FOLD_WIDTH;
}

void EditorMargin::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    const auto& theme = m_editor->theme();
    if (theme && event){
        QTextCharFormat frm;
        theme->format(frm, "Margin");

        QBrush back = frm.background();
        painter.fillRect(event->rect(), back);
        painter.setPen(back.color().darker());
        painter.drawLine(event->rect().topRight(), event->rect().bottomRight());

        QTextBlock block = m_editor->firstVisibleBlock();
        int top = static_cast<int>(m_editor->blockBoundingGeometry(block).translated(m_editor->contentOffset()).top());
        int bottom = top + static_cast<int>(m_editor->blockBoundingRect(block).height());

        painter.setPen(frm.foreground().color());

        while (block.isValid() && top <= event->rect().bottom()) {
            if (block.isVisible() && bottom >= event->rect().top()) {
                QRect rect = {0, top, event->rect().width() - 4, fontMetrics().height()};

                painter.drawText(
                    rect.adjusted(0, 0, -FOLD_WIDTH, 0),
                    Qt::AlignRight,
                    QString::number(block.blockNumber() + 1)
                );

                syntax::Folding* folding = syntax::folding(block);

                if (folding && folding->type == syntax::Folding::Begin){
                    QStyleOptionViewItem opt;
                    opt.rect = rect.adjusted(rect.width() - FOLD_WIDTH, 0, 0, 0);
                    opt.state = QStyle::State_Active | QStyle::State_Item | QStyle::State_Children;
                    if (!folding->closed)
                        opt.state |= QStyle::State_Open;

                    style()->drawPrimitive(QStyle::PE_IndicatorBranch, &opt, &painter, this);
                }
            }

            block   = block.next();
            top     = bottom;
            bottom  = top + static_cast<int>(m_editor->blockBoundingRect(block).height());
        }
    }
}

void EditorMargin::mousePressEvent(QMouseEvent * event)
{
    QTextCursor cursor = m_editor->cursorForPosition(QPoint(0, event->pos().y()));
    auto block = cursor.block();
    if (!block.isValid())
        return;

    syntax::Folding* folding = syntax::folding(block);
    if (folding && event->pos().x() > width() - FOLD_WIDTH){
        folding->closed = !folding->closed;

        int skip = 0;
        for(auto blk = block.next(); blk.isValid(); blk = blk.next()){
            if (skip){
                --skip;
                continue;
            }
            blk.setVisible(folding->closed ? false : true);

            syntax::Folding* innerFold = syntax::folding(blk);
            if (innerFold && innerFold->type == syntax::Folding::Begin && innerFold->closed){
                for(auto inner = blk.next(); inner.isValid(); inner = inner.next()){
                    skip++;
                    if (innerFold->checkEndBlock(inner))
                        break;
                }
            }

            if (folding->checkEndBlock(blk))
                break;
        }
        m_editor->document()->documentLayout()->update();
    }
}
