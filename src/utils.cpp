#include <QDebug>
#include <QKeySequence>
#include <QGuiApplication>
#include "utils.h"

QString Utils::shortcutText(QKeySequence::StandardKey seq, const QString& textSeq)
{
    auto keys = seq != 0 ? QKeySequence(seq) : QKeySequence(textSeq);
    return keys.toString();
}

int Utils::shortcutKey(QKeySequence::StandardKey seq, const QString& textSeq)
{
    auto keys = seq != 0 ? QKeySequence(seq) : QKeySequence(textSeq);

    if (keys.count() != 1)
        return 0;

    return keys[0] & ~modifiers(keys);
}

int Utils::shortcutModifiers(QKeySequence::StandardKey seq, const QString& textSeq)
{
    auto keys = seq != 0 ? QKeySequence(seq) : QKeySequence(textSeq);
    if (keys.count() != 1)
        return 0;

    return modifiers(keys);
}

int Utils::modifiers(const QKeySequence& seq)
{
    QStringList spl = seq.toString().split("+");
    int modifier = Qt::NoModifier;
    for(const auto& part: spl){
        if (part.toLower() == "ctrl")
            modifier |= Qt::ControlModifier;
        if (part.toLower() == "shift")
            modifier |= Qt::ShiftModifier;
        if (part.toLower() == "alt")
            modifier |= Qt::AltModifier;
        if (part.toLower() == "meta")
            modifier |= Qt::MetaModifier;
    }
    return modifier;
}

int Utils::globalModifiers()
{
    return QGuiApplication::queryKeyboardModifiers();
}
