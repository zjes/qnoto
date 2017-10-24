#pragma once
#include <QtPlugin>
#include "includes/plugin.h"

namespace qnoto {

FORWARD(ToolPlugin)

class COMMON_EXPORT ToolPlugin: public Plugin
{
    Q_OBJECT
public:
    virtual QWidget* create(QWidget* parent) const = 0;
};

}

Q_DECLARE_INTERFACE(qnoto::ToolPlugin, "qNoto.ToolPlugin")
