#pragma once
#include <QPlainTextEdit>
#include <QQuickPaintedItem>

class CodeEditor: public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QString text READ getText WRITE setText NOTIFY textChanged)
public:
    CodeEditor(QQuickItem* parent = nullptr);
protected:
    void paint(QPainter *painter) override;
private:
    const QString& getText() const;
    void setText(const QString& text);
signals:
    void textChanged();
private:
    QString m_text;
    QPlainTextEdit m_edit;
};
