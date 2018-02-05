#include <QFileInfo>
#include "fileinfo.h"

QString FileInfo::absolutePath(const QString& file)
{
    return QFileInfo(file).absoluteFilePath();
}

QString FileInfo::filename(const QString& file)
{
    return QFileInfo(file).fileName();
}

QString FileInfo::extention(const QString& file)
{
    return QFileInfo(file).suffix();
}
