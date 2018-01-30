#pragma once
#include <QObject>
#include <QMutex>
#include "common-export.h"

namespace qnoto {

class COMMON_EXPORT FileHandler: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList files READ openedFiles NOTIFY filesChanged)
public:
    static FileHandler& instance();
    ~FileHandler();

public slots:
    const QStringList& openedFiles() const;
    void close(const QString& fileName);
    void activate(const QString& fileName);
    void fileChange(const QString& fileName, bool modified);

signals:
    void activated(const QString& file);
    void closed(const QString& file);
    void modified(const QString& file, bool modified);
    void filesChanged();

private:
    FileHandler();

private:
    mutable QMutex m_mutex;
    QStringList    m_files;
};

}
