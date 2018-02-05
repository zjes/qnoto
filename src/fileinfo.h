#pragma once
#include <QObject>

class FileInfo: public QObject
{
    Q_OBJECT
public:
    using QObject::QObject;
public slots:
    static QString absolutePath(const QString& file);
    static QString filename(const QString& file);
    static QString extention(const QString& file);
};
