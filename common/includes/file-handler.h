#pragma once
#include <QObject>
#include <QMutex>
#include "common-export.h"

namespace qnoto {

class COMMON_EXPORT FileHandler: public QObject
{
    Q_OBJECT
public:
    static FileHandler& instance();
    ~FileHandler();
    const QStringList& openedFiles() const;
    void close(const QString& fileName);
    void activate(const QString& fileName);
signals:
    void activated(const QString& file);
    void closed(const QString& file);
    void modified(const QString& file, bool modified);
private:
    FileHandler();
private:
    mutable QMutex m_mutex;
    QStringList    m_files;
};

}
