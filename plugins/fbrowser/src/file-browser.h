#pragma once
#include "includes/plugins/leftside.h"

class ListWidget;

class FileBrowser: public qnoto::LeftSide
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "qNoto.LeftSide.FileBrowser")
    Q_INTERFACES(qnoto::LeftSide)
public:
    FileBrowser();
public:
    QString name() const override;
    QString title() const override;
    QWidget* create() const override;
};

