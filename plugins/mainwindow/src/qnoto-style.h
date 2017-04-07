#pragma once

#include <QProxyStyle>

class QStyleOptionToolButton;
class QNotoStyle: public QProxyStyle
{
    Q_OBJECT
public:
    QNotoStyle(QStyle *baseStyle = 0);
    virtual void polish(QWidget *widget);
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    virtual void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const;
    virtual void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;
    virtual int pixelMetric(PixelMetric metric, const QStyleOption *option, const QWidget *widget) const;
    virtual QSize sizeFromContents(ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const;
private:
    bool isPanel(const QWidget* widget) const;
    QPalette panelPalette(const QPalette &oldPalette, bool lightColored = false);
    void drawButtonSeparator(QPainter *painter, const QRect &rect, bool reverse, bool lr = false) const;
    bool isLight();
    void createPanelImage();
private slots:
    void resetStyle();
private:
    QPixmap m_panel;
    QColor m_hover;
    QColor m_panelColor;
};
