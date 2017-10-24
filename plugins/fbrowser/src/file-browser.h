#pragma once
#include "includes/plugins/tool-plugin.h"

class ListWidget;

class FileBrowser: public qnoto::ToolPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "qNoto.ToolPlugin.FileBrowser")
    Q_INTERFACES(qnoto::ToolPlugin)
public:
    FileBrowser();
public:
    QString name() const override;
    QString title() const override;
    QWidget* create(QWidget* parent) const override;
};

