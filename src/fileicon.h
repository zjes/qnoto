#pragma once
#include <QQuickImageProvider>

class FileIconProvider : public QQuickImageProvider
{
public:
    FileIconProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
};
