#pragma once
#include <QObject>
#include "common-export.h"

namespace qnoto {

class COMMON_EXPORT FileHandler: public QObject
{
    Q_OBJECT
public:
    static FileHandler& instance();
    ~FileHandler();
    const QStringList& openedFiles();
signals:
    void activated(const QString& file);
    void closed(const QString& file);
    void modified(const QString& file, bool modified);
private:
    FileHandler();
private:
    QStringList m_files;
};

}
