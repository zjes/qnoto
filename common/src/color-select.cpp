#include <QDebug>
#include <QPainter>
#include <QColorDialog>
#include <QStyle>
#include <QStyleOptionButton>
#include <QMenu>
#include "includes/color-select.h"

ColorSelect::ColorSelect(QWidget* parent):
    QToolButton(parent)
{
    QMenu * menu = new QMenu(this);
    menu->addAction(tr("Reset"), this, SLOT(colorReset()));
    setMenu(menu);
    setPopupMode(QToolButton::MenuButtonPopup);
    m_defaultColor = QColor(0, 0, 0, 0);
    setColor(m_defaultColor);
    connect(this, SIGNAL(clicked()), SLOT(showDialog()));
}

ColorSelect::~ColorSelect()
{
}

QColor ColorSelect::color()
{
    return m_color;
}

void ColorSelect::setColor(const QColor& color)
{
    m_color = color;
    setText(m_color.alpha() == 0 ? tr("Transparent") : m_color.name().toUpper());
}

void ColorSelect::setDefaultColor(const QColor& color)
{
    m_defaultColor = color;
}

bool ColorSelect::isLight()
{
    int yiq = ((m_color.red()*299)+(m_color.green()*587)+(m_color.blue()*114))/1000;
    return yiq >= 128;
}

void ColorSelect::paintEvent(QPaintEvent *evt)
{
    QToolButton::paintEvent(evt);
    if (m_color.alpha() != 0){
        QPainter paint(this);
        QStyleOptionButton option;
        option.initFrom(this);
        option.state = isDown() ? QStyle::State_Sunken : QStyle::State_Raised;
        option.text = text();
        option.icon = icon();
        int menuWidth = style()->pixelMetric(QStyle::PM_MenuButtonIndicator, &option, this);
        QRect drawRect = style()->subElementRect(QStyle::SE_PushButtonFocusRect, &option, this).adjusted(2, 0, -2 - menuWidth, -0);
        if (isEnabled()){
            paint.fillRect(drawRect, m_color);
        }
        paint.setPen(m_color.alpha() == 0 || isLight() ? Qt::black : Qt::white);
        paint.drawText(drawRect, Qt::AlignCenter, text());
    }
}

void ColorSelect::showDialog()
{
    QColorDialog dlg(m_color, this);
    if (dlg.exec() == QDialog::Accepted){
        setColor(dlg.currentColor());
        emit changed(m_color);
    }
}

QSize ColorSelect::sizeHint() const
{
    return QSize(60, 24);
}

void ColorSelect::colorReset()
{
    setColor(m_defaultColor);
}
