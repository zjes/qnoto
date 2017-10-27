#pragma once
#include <QStringList>
#include <QScopedPointer>
#include "includes/settings.h"

class Settings: public qnoto::Settings
{
public:
    static QStringList openedFiles();
    static void setOpenedFiles(const QStringList& list);
    static bool restoreState();
    static void setRestoreState(bool restore);
    static QString lastDir();
    static void setLastDir(const QString& last);
    static void addToRecent(const QString& file);
    static QStringList recentFiles();
private:
    static Settings& instance();
    Settings();
};
