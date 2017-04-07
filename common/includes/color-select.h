#pragma once
#include <QToolButton>
#include "common-export.h"

class COMMON_EXPORT ColorSelect: public QToolButton
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)
public:
    ColorSelect(QWidget* parent);
    virtual ~ColorSelect();

    QColor color();
    void setColor(const QColor& color);
    void setDefaultColor(const QColor& color);
public slots:
    void colorReset();
    void showDialog();
signals:
    void changed(const QColor& color);
protected:
    virtual void paintEvent(QPaintEvent *);
    virtual QSize sizeHint() const;
private:
    bool isLight();
private:
    QColor m_color;
    QColor m_defaultColor;
};
