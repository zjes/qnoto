#pragma once
#include <QScopedPointer>
#include "includes/settings.h"

class QSettings;
class QFont;

class Settings: public qnoto::Settings
{
public:
    static Settings& instance();
public:
    QFont font();
    void setFont(const QFont& font);

public:
    qnoto::Property<bool>    wordWrap;
    qnoto::Property<QString> theme;
    qnoto::Property<bool>    showLineNumbers;
    qnoto::Property<bool>    showWhite;
    qnoto::Property<bool>    showEndl;
    qnoto::Property<bool>    antialiasedFont;
    qnoto::Property<bool>    hightlightLine;

private:
    Settings();
};
