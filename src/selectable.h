#pragma once
#include <QQuickPaintedItem>
#include <QQuickTextDocument>

class Selectable: public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
public:
    Selectable(QQuickItem *parent = nullptr);


    void paint(QPainter *painter) override;
signals:
    void textChanged();

protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    QString text();
    void setText(const QString& text);
private:
    QQuickTextDocument m_text;
};
