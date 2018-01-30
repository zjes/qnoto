#include <QDebug>
#include "includes/file-handler.h"

namespace qnoto {

FileHandler& FileHandler::instance()
{
    static FileHandler fh;
    return fh;
}

FileHandler::FileHandler()
{
}

void FileHandler::close(const QString& fileName)
{
    {
        QMutexLocker locker(&m_mutex);
        m_files.removeAll(fileName);
    }
    emit closed(fileName);
    emit filesChanged();
    if (m_files.size())
        emit activated(m_files[0]);
}

void FileHandler::activate(const QString& fileName)
{
    {
        QMutexLocker locker(&m_mutex);
        m_files.removeAll(fileName);
        m_files.prepend(fileName);
    }
    emit filesChanged();
    emit activated(fileName);
}

FileHandler::~FileHandler()
{}

const QStringList& FileHandler::openedFiles() const
{
    QMutexLocker locker(&m_mutex);
    return m_files;
}

void FileHandler::fileChange(const QString& fileName, bool mod)
{
    emit modified(fileName, mod);
}

}
