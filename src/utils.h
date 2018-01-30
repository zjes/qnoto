#pragma once
#include <QObject>
#include <QKeySequence>

class Utils: public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
public slots:
    QString shortcutText(QKeySequence::StandardKey seq, const QString& textSeq);
    int shortcutKey(QKeySequence::StandardKey seq, const QString& textSeq);
    int shortcutModifiers(QKeySequence::StandardKey seq, const QString& textSeq);
    int globalModifiers();
private:
    int modifiers(const QKeySequence& seq);
};
