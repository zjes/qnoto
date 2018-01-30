#include <QFileInfo>
#include <QDir>
#include <QDebug>
#include "file-pathmodel.h"

FilePathModel::FilePathModel(const QString& path)
{
    QDir dir(path);
    auto name = [](const QDir& p){
        QString ret = !p.dirName().isEmpty() ? p.dirName() : p.path();
        if (ret.endsWith("/"))
            ret = ret.left(ret.length()-1);
        return ret;
    };

    m_data.append({dir.absolutePath(), name(dir)});

    while(dir.cdUp()){
        m_data.prepend({dir.absolutePath(), name(dir)});
    }
    m_data.prepend({"", "Root"});
}

int FilePathModel::rowCount(const QModelIndex&) const
{
    return m_data.size();
}

QVariant FilePathModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return {};
    const Info& info = m_data.at(index.row());
    switch (role) {
    case Qt::UserRole + 1:
        return info.fileName;
    case Qt::UserRole + 2:
        return info.path;
    }
    return {};
}

QHash<int, QByteArray> FilePathModel::roleNames() const
{
    return {
        {Qt::UserRole + 1, "name"},
        {Qt::UserRole + 2, "path"},
    };
}
