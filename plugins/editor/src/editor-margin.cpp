#include <QDebug>
#include <QPainter>
#include <QTextBlock>
#include <QTimer>

#include "editor-margin.h"
#include "editor-impl.h"
#include "syntax/theme.h"

EditorMargin::EditorMargin(EditorImpl* editor):
    QWidget(editor),
    m_editor(editor)
{
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
    return 4 + fontMetrics().width('0') * QString::number(m_editor->blockCount()+1).length() + 4;
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
        int blockNumber = block.blockNumber();
        int top = static_cast<int>(m_editor->blockBoundingGeometry(block).translated(m_editor->contentOffset()).top());
        int bottom = top + static_cast<int>(m_editor->blockBoundingRect(block).height());

        while (block.isValid() && top <= event->rect().bottom()) {
            if (block.isVisible() && bottom >= event->rect().top()) {
                QString number = QString::number(blockNumber + 1);
                painter.setPen(frm.foreground().color());
                painter.drawText(0, top, event->rect().width() - 4, fontMetrics().height(), Qt::AlignRight, number);
            }

            block = block.next();
            top = bottom;
            bottom = top + static_cast<int>(m_editor->blockBoundingRect(block).height());
            blockNumber = block.blockNumber();
        }
    }
}
