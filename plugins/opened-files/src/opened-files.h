#pragma once
#include "includes/plugins/tool-plugin.h"

class QListWidget;

class OpenedFiles: public qnoto::ToolPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "qNoto.ToolPlugin.OpenedFiles")
    Q_INTERFACES(qnoto::ToolPlugin)
public:
    QString name() const override;
    QString title() const override;
    QWidget* create(QWidget* parent) const override;
};
