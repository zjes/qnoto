#include <QApplication>
#include <QDebug>
#include <QWidget>
#include <QVariant>
#include <QDockWidget>
#include <QToolButton>
#include <QLineEdit>
#include <QLabel>
#include <QStatusBar>
#include <QComboBox>
#include <QPainter>
#include <QTimer>
#include <QPushButton>

#include "qnoto-style.h"

QNotoStyle::QNotoStyle(QStyle *baseStyle):
    QProxyStyle(baseStyle)
{
    m_panelColor = QColor("#303030");
    createPanelImage();
}

bool QNotoStyle::isPanel(const QWidget* widget) const
{
    if (!widget)
        return false;

    const QWidget *w = widget;
    while(w){
        if (w && (w->property("panelWidget").toBool() || w->property("panelWidget2").toBool())){
            return true;
        }
        w = w->parentWidget();
    }
    return false;
}

QPalette QNotoStyle::panelPalette(const QPalette &oldPalette, bool /*lightColored*/)
{
    QColor color("#FFFFFF");
    QPalette pal = oldPalette;
    pal.setBrush(QPalette::All, QPalette::WindowText, color);
    pal.setBrush(QPalette::All, QPalette::ButtonText, color);
    pal.setBrush(QPalette::All, QPalette::Foreground, color);
    color.setAlpha(100);
    pal.setBrush(QPalette::Disabled, QPalette::WindowText, color);
    pal.setBrush(QPalette::Disabled, QPalette::ButtonText, color);
    pal.setBrush(QPalette::Disabled, QPalette::Foreground, color);
    return pal;
}

void QNotoStyle::polish(QWidget *widget)
{
    QProxyStyle::polish(widget);

    if (isPanel(widget)){
        if (qobject_cast<QDockWidget*>(widget))
            widget->setContentsMargins(0, 0, 0, 0);

        widget->setAttribute(Qt::WA_LayoutUsesWidgetRect, true);
        if (qobject_cast<QToolButton*>(widget)) {
            widget->setAttribute(Qt::WA_Hover);
            widget->setMaximumHeight(23);
        }
        else if (qobject_cast<QLineEdit*>(widget)) {
            widget->setAttribute(Qt::WA_Hover);
            widget->setMaximumHeight(23);
        }
        else if (qobject_cast<QLabel*>(widget))
            widget->setPalette(panelPalette(widget->palette()));
        else if (qobject_cast<QStatusBar*>(widget))
            widget->setFixedHeight(25);
        else if (qobject_cast<QComboBox*>(widget)) {
            widget->setMaximumHeight(23);
            widget->setAttribute(Qt::WA_Hover);
        }
        if (widget->property("panelWidget").toBool()) {
            widget->setMaximumHeight(25);
            widget->setMinimumHeight(25);
            widget->setBackgroundRole(QPalette::Background);
            widget->setAutoFillBackground(true);
            QPalette pal = widget->palette();
            for (int i = 0; i < QPalette::NColorGroups; ++i) {
                pal.setBrush(QPalette::ColorGroup(i), QPalette::Background, m_panel);
            }
            widget->setPalette(pal);
        }
        if (widget->property("panelWidget2").toBool()) {
            widget->setBackgroundRole(QPalette::Background);
            widget->setAutoFillBackground(true);
            QPalette pal = widget->palette();
            pal.setColor(QPalette::Background, m_panelColor);
            widget->setPalette(pal);
        }
        QPalette pal = widget->palette();
        pal.setColor(QPalette::WindowText, isLight() ? Qt::black : Qt::white);
        pal.setColor(QPalette::ButtonText, isLight() ? Qt::black : Qt::white);
        widget->setPalette(pal);
    }
}

void QNotoStyle::drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (!isPanel(widget))
        return QProxyStyle::drawPrimitive(element, option, painter, widget);

    switch (element) {
    case PE_PanelStatusBar:
        {
            painter->save();
            //painter->drawPixmap(rect, m_panel);
            painter->restore();
        }
        break;
    case PE_PanelButtonTool:
        {
            painter->save();
            bool hovered = option->state & State_Enabled && option->state & State_MouseOver;
            if (hovered) {
                int rshift = 0;
                if (qobject_cast<const QComboBox*>(widget))
                    rshift = -2;
                painter->fillRect(option->rect.adjusted(0, 0, rshift, 0), m_hover);
                if (qobject_cast<const QToolButton*>(widget) || qobject_cast<const QPushButton*>(widget)){
                    painter->drawRect(option->rect.adjusted(0, 0, rshift-1, -1));
                }
            }
            if (qobject_cast<const QComboBox*>(widget)){
                drawButtonSeparator(painter, option->rect, false);
            }
            if (qobject_cast<const QPushButton*>(widget)){
                drawButtonSeparator(painter, option->rect, false, true);
            }
            painter->restore();
        }
        break;
    default:
        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
}

void QNotoStyle::drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const
{
    if (!isPanel(widget))
        return QProxyStyle::drawComplexControl(control, option, painter, widget);

    switch(control){
    case CC_ToolButton:
        if (const QStyleOptionToolButton *toolbutton = qstyleoption_cast<const QStyleOptionToolButton *>(option)) {
            if (widget->property("fancy").toBool()){
                //drawFancyToolButton(toolbutton, painter, widget);
            } else {
                painter->save();
                bool pressed = toolbutton->state & State_Sunken || toolbutton->state & State_On;
                bool hovered = toolbutton->state & State_Enabled && toolbutton->state & State_MouseOver;
                drawPrimitive(PE_PanelButtonTool, option, painter, widget);
                QRect iconRect = pressed ? toolbutton->rect.adjusted(2, 1, 2, 2) : toolbutton->rect.adjusted(1, 0, 1, 1);
                QIcon::Mode mode = QIcon::Normal;
                if (!(toolbutton->state & State_Enabled))
                    mode = QIcon::Disabled;
                else if (hovered && pressed)
                    mode = QIcon::Active;
                toolbutton->icon.paint(painter, iconRect, Qt::AlignCenter, mode);
                if (!toolbutton->text.isEmpty()){
                    int shift = toolbutton->icon.isNull() ? 0 : toolbutton->iconSize.width()+5;
                    QPalette pal = widget->palette();
                    painter->setPen((option->state & State_Enabled) ? pal.color(QPalette::ButtonText) : pal.color(QPalette::ButtonText).lighter());
                    painter->drawText(option->rect.adjusted(shift, 0, 0, 0), toolbutton->text);
                }
                painter->restore();
            }
        }
        break;
    case CC_ComboBox:
        if (const QStyleOptionComboBox *cb = qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
            painter->save();
            drawPrimitive(PE_PanelButtonTool, option, painter, widget);
            int aw = 12;
            QIcon(":/icons/arrowdown.png").paint(painter, cb->rect.adjusted(cb->rect.width()-aw, 0, -4, 0), Qt::AlignCenter);
            painter->restore();
        }
        break;
    default:
        QProxyStyle::drawComplexControl(control, option, painter, widget);
    }
}

void QNotoStyle::drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
{
    if (element == CE_Splitter){
        painter->fillRect(option->rect, QColor("#000000"));
    }
    if (!isPanel(widget))
        return QProxyStyle::drawControl(element, option, painter, widget);

    switch(element){
    case CE_ComboBoxLabel:
        if (const QStyleOptionComboBox *cb = qstyleoption_cast<const QStyleOptionComboBox *>(option)) {
            painter->save();
            QRect editRect = subControlRect(CC_ComboBox, cb, SC_ComboBoxEditField, widget);
            QString text = option->fontMetrics.elidedText(cb->currentText, Qt::ElideRight, editRect.width());
            QPalette pal = widget->palette();
            if ((option->state & State_Enabled)) {
                painter->setPen(pal.color(QPalette::WindowText));
                painter->drawText(editRect.adjusted(1, 0, -1, 0), Qt::AlignLeft | Qt::AlignVCenter, text);
            } else {
                painter->setOpacity(0.8);
                painter->setPen(pal.color(QPalette::WindowText).lighter());
                painter->drawText(editRect.adjusted(1, 0, -1, 0), Qt::AlignLeft | Qt::AlignVCenter, text);
            }
            painter->restore();
        }
        break;
    case CE_PushButton:
        if (isPanel(widget)){
            const QPushButton *btn = qobject_cast<const QPushButton*>(widget);
            painter->save();
            drawPrimitive(PE_PanelButtonTool, option, painter, widget);

            const QStyleOptionButton *button = qstyleoption_cast<const QStyleOptionButton *>(option);

            bool pressed = button->state & State_Sunken || button->state & State_On;
            bool hovered = button->state & State_Enabled && button->state & State_MouseOver;

            int shift = 0;
            if (!button->icon.isNull()){
                QRect iconRect = pressed ?
                            QRect(option->rect.left()+3, option->rect.top()+3, option->rect.height(), option->rect.height()-4):
                            QRect(option->rect.left()+2, option->rect.top()+2, option->rect.height(), option->rect.height()-4);
                QIcon::Mode mode = QIcon::Normal;
                if (!(button->state & State_Enabled))
                    mode = QIcon::Disabled;
                else if (hovered && pressed)
                    mode = QIcon::Active;
                button->icon.paint(painter, iconRect, Qt::AlignCenter, mode);
                shift = iconRect.width();
            }

            painter->drawText(option->rect.adjusted(shift, 0, 0, 0), Qt::AlignCenter, btn->text());
            painter->restore();
        } else {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    default:
        if (!isPanel(widget))
            QProxyStyle::drawControl(element, option, painter, widget);
    }
}

int QNotoStyle::pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const
{
    int retval = 0;
    retval = QProxyStyle::pixelMetric(metric, option, widget);
    switch (metric) {
    case PM_SplitterWidth:
        //if (widget && widget->property("minisplitter").toBool())
        retval = 1;
        break;
    case PM_DefaultFrameWidth:
        if (qobject_cast<const QLineEdit*>(widget) && isPanel(widget))
            return 1;
        break;
    default:
        break;
    }
    return retval;
}

QSize QNotoStyle::sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
{
    QSize newSize = QProxyStyle::sizeFromContents(type, option, size, widget);

    if (type == CT_Splitter && widget)// && widget->property("minisplitter").toBool()
        return QSize(1, 1);
    return newSize;
}

void QNotoStyle::drawButtonSeparator(QPainter *painter, const QRect &rect, bool reverse, bool lr) const
{
    QLinearGradient grad(rect.topRight(), rect.bottomRight());
    grad.setColorAt(0, QColor(255, 255, 255, 20));
    grad.setColorAt(0.4, QColor(255, 255, 255, 60));
    grad.setColorAt(0.7, QColor(255, 255, 255, 50));
    grad.setColorAt(1, QColor(255, 255, 255, 40));
    painter->setPen(QPen(grad, 0));
    painter->drawLine(rect.topRight(), rect.bottomRight());
    grad.setColorAt(0, QColor(0, 0, 0, 30));
    grad.setColorAt(0.4, QColor(0, 0, 0, 70));
    grad.setColorAt(0.7, QColor(0, 0, 0, 70));
    grad.setColorAt(1, QColor(0, 0, 0, 40));
    painter->setPen(QPen(grad, 0));
    if (!reverse)
        painter->drawLine(rect.topRight() - QPoint(1,0), rect.bottomRight() - QPoint(1,0));
    else {
        painter->drawLine(rect.topLeft(), rect.bottomLeft());
    }
    if (lr){
        grad.setColorAt(0, QColor(255, 255, 255, 20));
        grad.setColorAt(0.4, QColor(255, 255, 255, 60));
        grad.setColorAt(0.7, QColor(255, 255, 255, 50));
        grad.setColorAt(1, QColor(255, 255, 255, 40));
        painter->setPen(QPen(grad, 0));
        painter->drawLine(rect.topLeft() + QPoint(1,0), rect.bottomLeft() + QPoint(1,0));
        grad.setColorAt(0, QColor(0, 0, 0, 30));
        grad.setColorAt(0.4, QColor(0, 0, 0, 70));
        grad.setColorAt(0.7, QColor(0, 0, 0, 70));
        grad.setColorAt(1, QColor(0, 0, 0, 40));
        painter->setPen(QPen(grad, 0));
        painter->drawLine(rect.topLeft(), rect.bottomLeft());
    }
}

void QNotoStyle::resetStyle()
{
    for(QWidget *w: QApplication::allWidgets()){
        polish(w);
        w->update();
    }
}

bool QNotoStyle::isLight()
{
    int yiq = ((m_panelColor.red()*299)+(m_panelColor.green()*587)+(m_panelColor.blue()*114))/1000;
    return yiq >= 128;
}

void QNotoStyle::createPanelImage()
{
    QPixmap p(":/images/panel.png");
    m_panel = QPixmap(p.width(), p.height());
    m_panel.fill(m_panelColor);
    QPainter ppanel(&m_panel);
    ppanel.drawPixmap(0, 0, p);
    ppanel.end();
}
