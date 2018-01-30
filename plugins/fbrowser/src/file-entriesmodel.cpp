#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include "file-entriesmodel.h"

FileEntriesModel::FileEntriesModel(const QString& path)
{
    if (path.isEmpty()){
        if (QSysInfo::productType() == "windows"){
            for(const auto& dr: QDir::drives()){
                m_data.append({dr.absolutePath(), dr.path(), true});
            }
        }
    } else {
        QDir dir(path);
        dir.setFilter(QDir::NoDot | QDir::AllEntries | QDir::Drives);
        dir.setSorting(QDir::DirsFirst | QDir::IgnoreCase);
        for(QFileInfo info: dir.entryInfoList())
        {
            m_data.append({info.absoluteFilePath(), info.fileName(), info.isDir()});
        }
        if (m_data.empty()){
            dir.cdUp();
            m_data.append({dir.path(), "..", true});
        }
    }
}

int FileEntriesModel::rowCount(const QModelIndex&) const
{
    return m_data.size();
}

QVariant FileEntriesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};
    const Info& info = m_data.at(index.row());
    switch (role) {
    case Qt::UserRole + 1:
        return info.fileName;
    case Qt::UserRole + 2:
        return info.path;
    case Qt::UserRole + 3:
        return info.isDir;
    }
    return {};
}

QHash<int, QByteArray> FileEntriesModel::roleNames() const
{
    return {
        {Qt::UserRole + 1, "name"},
        {Qt::UserRole + 2, "path"},
        {Qt::UserRole + 3, "isDir"},
    };
}
