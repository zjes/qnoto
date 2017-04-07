#pragma once
#include "includes/plugins/leftside.h"

class QListWidget;

class OpenedFiles: public qnoto::LeftSide
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "qNoto.LeftSide.OpenedFiles")
    Q_INTERFACES(qnoto::LeftSide)
public:
    QString name() const override;
    QString title() const override;
    QWidget* create() const override;
};
