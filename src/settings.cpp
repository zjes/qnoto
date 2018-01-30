#include <QDebug>
#include <QQmlEngine>
#include "settings.h"
#include "plugins-model.h"
#include "includes/file-handler.h"


Settings::Settings():
    qnoto::Settings("qnoto")
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    connect(&qnoto::FileHandler::instance(), &qnoto::FileHandler::activated, this, &Settings::openFile);
    connect(&qnoto::FileHandler::instance(), &qnoto::FileHandler::closed, this, &Settings::closeFile);
}

bool Settings::restoreState() const
{
    return value("restoreState", true);
}

QStringList Settings::files() const
{
    return value<QStringList>("files");
}

void Settings::openFile(const QString& file)
{
    auto fs = files();
    fs.removeAll(file);
    fs.prepend(file);
    setValue("files", fs);
    save();
}

void Settings::closeFile(const QString& file)
{
    auto fs = files();
    fs.removeAll(file);
    setValue("files", fs);
    save();
}
