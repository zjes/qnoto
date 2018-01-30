#pragma once
#include <QAbstractListModel>

class QQmlComponent;
class FileHandler: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString current READ getCurrent NOTIFY currentChanged)
    Q_PROPERTY(QAbstractItemModel* path READ pathModel NOTIFY currentChanged)
    Q_PROPERTY(QAbstractItemModel* entries READ model NOTIFY currentChanged)
public:
    FileHandler();

public slots:
    QAbstractItemModel* model(const QString& path = {});
    QAbstractItemModel* pathModel();

    const QString& getCurrent() const;
    QAbstractItemModel* chdir(const QString& path);

signals:
    void currentChanged();

private:
    QString m_rootPath;
};

