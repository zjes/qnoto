#include "includes/file-handler.h"

namespace qnoto {

FileHandler& FileHandler::instance()
{
    static FileHandler fh;
    return fh;
}

FileHandler::FileHandler()
{
    connect(this, &FileHandler::activated, [this](const QString& file){
        m_files.removeAll(file);
        m_files.prepend(file);
    });
    connect(this, &FileHandler::closed, [this](const QString& file){
        m_files.removeAll(file);
    });
}

FileHandler::~FileHandler()
{}

const QStringList& FileHandler::openedFiles()
{
    return m_files;
}

}
