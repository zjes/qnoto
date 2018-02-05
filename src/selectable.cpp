#include <QCursor>
#include <QPainter>
#include "selectable.h"

Selectable::Selectable(QQuickItem *parent):
    QQuickPaintedItem(parent),
    m_text(this)
{
    setCursor(Qt::IBeamCursor);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
}

QString Selectable::text()
{
    return m_text.textDocument()->toPlainText();
}

void Selectable::setText(const QString& text)
{
    if (text == m_text.textDocument()->toPlainText())
        return;
    m_text.textDocument()->setPlainText(text);
    emit textChanged();
}

void Selectable::paint(QPainter* painter)
{
    m_text.textDocument()->drawContents(painter);
}

void Selectable::mouseMoveEvent(QMouseEvent *event)
{
    qDebug() << "mouse move";
    if (event->buttons() & Qt::LeftButton){
        qDebug() << "mouse move with lbtn";
    }
    QQuickPaintedItem::mouseMoveEvent(event);
}

void Selectable::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mouse press" << event->button();
}
