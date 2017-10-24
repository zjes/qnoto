#pragma once
#include <QStringList>
#include <QScopedPointer>

class QSettings;
class Settings
{
public:
    ~Settings();
public:
    static QStringList openedFiles();
    static void setOpenedFiles(const QStringList& list);
    static bool restoreState();
    static void setRestoreState(bool restore);
    static QString lastDir();
    static void setLastDir(const QString& last);
private:
    static Settings& instance();
    Settings();
private:
    QScopedPointer<QSettings> m_setts;
};
