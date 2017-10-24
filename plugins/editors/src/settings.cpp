#include <QSettings>
#include "settings.h"

Settings::Settings():
    m_setts(new QSettings(QSettings::IniFormat, QSettings::UserScope, "qnoto", "editors"))
{}

Settings& Settings::instance()
{
    static Settings sets;
    return sets;
}

Settings::~Settings()
{}

QStringList Settings::openedFiles()
{
    QStringList ret;
    int size = instance().m_setts->beginReadArray("OpenedFiles");
    for(int i = size-1; i >= 0; --i){
        instance().m_setts->setArrayIndex(i);

        ret.append(instance().m_setts->value("file").toString());
    }
    instance().m_setts->endArray();
    return ret;
}

void Settings::setOpenedFiles(const QStringList& list)
{
    instance().m_setts->beginWriteArray("OpenedFiles");
    int count = 0;
    for(const QString& file: list){
        instance().m_setts->setArrayIndex(count++);
        instance().m_setts->setValue("file", file);
    }
    instance().m_setts->endArray();
}

bool Settings::restoreState()
{
    return instance().m_setts->value("restoreState", true).toBool();
}

void Settings::setRestoreState(bool restore)
{
    instance().m_setts->setValue("restoreState", restore);
}

QString Settings::lastDir()
{
    return instance().m_setts->value("lastDir").toString();
}

void Settings::setLastDir(const QString& last)
{
    instance().m_setts->setValue("lastDir", last);
}


