#include <QDebug>
#include <QVBoxLayout>
#include <QResource>
#include <QtQml>
#include <QQuickItem>
#include "file-browser.h"
#include "file-handler.h"

FileBrowser::FileBrowser()
{
    QResource::registerResource("plugins/fbrowser-res.rcc");
}

FileBrowser::~FileBrowser()
{
}

QString FileBrowser::name() const
{
    return "file-browser";
}

QString FileBrowser::title() const
{
    return tr("File browser");
}

QObject* FileBrowser::create(QObject* parent)
{
    qnoto::Component *component = createComponent(parent, "qrc:/qml/FileBrowser.qml");
    component->setContext("fileBrowser", new FileHandler);

    return component;
}
