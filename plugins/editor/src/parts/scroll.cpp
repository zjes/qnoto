#include <QPainter>
#include <QStyleOption>
#include "scroll.h"

void Scroll::setPosses(const QList<int>& posses, int count)
{
    m_posses = posses;
    m_count = count;
    update();
}

void Scroll::paintEvent(QPaintEvent* event)
{
    QScrollBar::paintEvent(event);
    QPainter p(this);

    if (m_count && m_posses.size()){
        QStyleOptionSlider opt;
        initStyleOption(&opt);
        QRect groove = style()->subControlRect(QStyle::CC_ScrollBar, &opt, QStyle::SC_ScrollBarGroove, this);

        p.setPen("#00aa00");
        p.setBrush(QColor("#00aa00"));
        qreal pxs = static_cast<qreal>(groove.height())/m_count;

        for(int pos: m_posses){
            p.drawRect(5, static_cast<int>(groove.top()+pxs*pos), groove.width()-10, 2);
        }
    }
}
