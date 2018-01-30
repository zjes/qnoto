#include <QDebug>
#include <QStandardPaths>
#include <QFileSystemModel>
#include <QFileIconProvider>
#include "file-handler.h"
#include "file-entriesmodel.h"
#include "file-pathmodel.h"
#include "settings.h"

FileHandler::FileHandler()
{
    m_rootPath = Settings::lastDir();
}

QAbstractItemModel* FileHandler::model(const QString& path)
{
    if (!path.isEmpty())
        m_rootPath = path;

    return new FileEntriesModel(m_rootPath);
}

const QString& FileHandler::getCurrent() const
{
    return m_rootPath;
}

QAbstractItemModel* FileHandler::chdir(const QString& path)
{
    m_rootPath = path;
    Settings::setLastDir(path);

    emit currentChanged();
    return new FileEntriesModel(m_rootPath);
}

QAbstractItemModel* FileHandler::pathModel()
{
    return new FilePathModel(m_rootPath);
}
