#include <QStandardPaths>
#include "settings.h"

Settings::Settings():
    qnoto::Settings("fbrowser")
{}

QString Settings::lastDir()
{
    QString def = QStandardPaths::standardLocations(QStandardPaths::HomeLocation)[0];
    return instance().value("last-dir", def);
}

void Settings::setLastDir(const QString& dir)
{
    instance().setValue("last-dir", dir);
}

Settings& Settings::instance()
{
    static Settings inst;
    return inst;
}

