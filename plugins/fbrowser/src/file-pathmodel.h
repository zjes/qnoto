#pragma once
#include <QAbstractListModel>


class FilePathModel: public QAbstractListModel
{
public:
    struct Info
    {
        QString path;
        QString fileName;
    };

public:
    FilePathModel(const QString& path);

protected:
    int rowCount(const QModelIndex& = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Info> m_data;
};
