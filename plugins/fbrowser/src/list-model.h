#pragma once

#include <QFileSystemModel>
#include <QSortFilterProxyModel>

class FileListModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit FileListModel(QObject *parent = 0);
    virtual int	columnCount(const QModelIndex & parent = QModelIndex()) const;
};

class ListModel: public QSortFilterProxyModel
{
    Q_OBJECT
public:
    ListModel(QObject *parent = 0);
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
    void setRootPath(const QString & path);
    QModelIndex index(const QString & path) const;
    QFileInfo fileInfo(const QModelIndex &index) const;
};
