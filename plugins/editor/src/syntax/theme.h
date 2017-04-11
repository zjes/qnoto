#pragma once
#include <QString>
#include <QMap>
#include <QColor>
#include "common.h"

class QTextCharFormat;

namespace syntax {

FORWARD(Theme)

struct Style
{
    QColor color          = QColor::Invalid;
    QColor selColor       = QColor::Invalid;
    QColor backColor      = QColor::Invalid;
    bool   bold           = false;
    bool   italic         = false;
    bool   underline      = false;
    bool   strikeThrough  = false;
    QColor borderColor    = QColor::Invalid;
    int    borderRadius   = 0;
};

class Theme
{
public:
    Theme();
    bool load(const QString& file);

    void format(QTextCharFormat& format, const QString& name = {}, const QString& style = {}) const;

    QColor color() const;
    QColor backgroud() const;
private:
    QMap<QString, Style> m_txtStyles;
    QMap<QString, Style> m_txtStyleNums;
    QMap<QString, QColor> m_default;
};

}
