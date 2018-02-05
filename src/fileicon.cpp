#include <QDebug>
#include <QFileIconProvider>
#include "fileicon.h"

FileIconProvider::FileIconProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap FileIconProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QFileIconProvider prov;
    QIcon icon = prov.icon(QUrl::fromPercentEncoding(id.toUtf8()));

    QSize newSize(requestedSize.width() > 0 ? requestedSize.width() : 22,
                  requestedSize.height() > 0 ? requestedSize.height() : 22);

    *size = newSize;

    return icon.pixmap(newSize);
}
