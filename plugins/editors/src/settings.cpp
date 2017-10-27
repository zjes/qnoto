#include <QSettings>
#include "settings.h"

Settings::Settings():
    qnoto::Settings("editors")
{}

Settings& Settings::instance()
{
    static Settings sets;
    return sets;
}

QStringList Settings::openedFiles()
{
    return instance().value<QStringList>("opened-files");
}

void Settings::setOpenedFiles(const QStringList& list)
{
    instance().setValue("opened-files", list);
}

bool Settings::restoreState()
{
    return instance().value("restoreState", true);
}

void Settings::setRestoreState(bool restore)
{
    instance().setValue("restoreState", restore);
}

QString Settings::lastDir()
{
    return instance().value("lastDir");
}

void Settings::setLastDir(const QString& last)
{
    instance().setValue("lastDir", last);
}

void Settings::addToRecent(const QString& file)
{
    QStringList recents = recentFiles();
    recents.removeAll(file);
    recents.prepend(file);

    instance().setValue("recent-files", recents);
}

QStringList Settings::recentFiles()
{
    return instance().value<QStringList>("recent-files");
}
