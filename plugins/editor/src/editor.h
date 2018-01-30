#pragma once
#include <QPlainTextEdit>
#include "includes/plugin.h"

class Editor: public qnoto::Plugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "qNoto.Editor.DefaultEditor")
    Q_INTERFACES(qnoto::Plugin)
public:
    Editor();
    QString name() const override;
    QString title() const override;
    QObject* create(QObject* parent) override;
};
