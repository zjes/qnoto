#include <QDebug>
#include "list-model.h"

FileListModel::FileListModel(QObject *parent) :
    QFileSystemModel(parent)
{
    QString root = myComputer().toString();
    setRootPath(root);
}

int	FileListModel::columnCount(const QModelIndex & /*parent*/) const
{
    return 1;
}

//--------------------------------------------------------------------------------------------------

ListModel::ListModel(QObject *parent):
    QSortFilterProxyModel(parent)
{
    setSourceModel(new FileListModel(this));
}

bool ListModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (sortColumn() == 0) {
        QFileSystemModel *fsm = qobject_cast<QFileSystemModel*>(sourceModel());
        bool asc = sortOrder() == Qt::AscendingOrder ? true : false;

        QFileInfo leftFileInfo  = fsm->fileInfo(left);
        QFileInfo rightFileInfo = fsm->fileInfo(right);

        if (sourceModel()->data(left).toString() == "..")
            return asc;
        if (sourceModel()->data(right).toString() == "..")
            return !asc;

        if (!leftFileInfo.isDir() && rightFileInfo.isDir()) {
            return !asc;
        }
        if (leftFileInfo.isDir() && !rightFileInfo.isDir()) {
            return asc;
        }
    }

    return QSortFilterProxyModel::lessThan(left, right);
}

void ListModel::setRootPath(const QString & path)
{
    qobject_cast<FileListModel*>(sourceModel())->setRootPath(path);
}

QModelIndex ListModel::index(const QString & path) const
{
    return mapFromSource(qobject_cast<FileListModel*>(sourceModel())->index(path));
}

QFileInfo ListModel::fileInfo(const QModelIndex &index) const
{
    return qobject_cast<FileListModel*>(sourceModel())->fileInfo(mapToSource(index));
}
