#pragma once
#include "includes/plugin.h"

class ListWidget;

class FileBrowser: public qnoto::Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "qNoto.ToolPlugin.FileBrowser")
    Q_INTERFACES(qnoto::Plugin)
public:
    FileBrowser();
    ~FileBrowser() override;
public:
    QString name() const override;
    QString title() const override;
    QObject* create(QObject* parent) override;
};

