#pragma once
#include "includes/settings.h"

class Settings: public qnoto::Settings
{
public:
    Settings();
public:
    static QString lastDir();
    static void    setLastDir(const QString& dir);
private:
    static Settings& instance();
};
