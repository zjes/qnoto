#include <QDebug>
#include <QVBoxLayout>
#include "file-browser.h"
#include "list-widget.h"

FileBrowser::FileBrowser()
{}


QString FileBrowser::name() const
{
    return "file-browser";
}

QString FileBrowser::title() const
{
    return tr("File browser");
}

QWidget* FileBrowser::create() const
{
    return new ListWidget;
}

