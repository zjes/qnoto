#include <QDebug>
#include <QSGSimpleRectNode>
#include <QPainter>
#include "codeeditor.h"

CodeEditor::CodeEditor(QQuickItem* parent):
    QQuickPaintedItem(parent)
{
    qDebug() << "create editor";
    setFlag(QQuickItem::ItemHasContents, true);
}

void CodeEditor::paint(QPainter* /*painter*/)
{
    qDebug() << "paint";
}

//QSGNode* CodeEditor::updatePaintNode(QSGNode* oldNode, UpdatePaintNodeData* /*updatePaintNodeData*/)
//{
//    QSGSimpleRectNode *n = static_cast<QSGSimpleRectNode *>(oldNode);
//    if (!n) {
//        n = new QSGSimpleRectNode();
//        n->setColor(Qt::red);
//    }
//    n->setRect(boundingRect());
//    return node;
//}

const QString& CodeEditor::getText() const
{
    return m_text;
}

void CodeEditor::setText(const QString& text)
{
    m_text = text;
    update();
}

