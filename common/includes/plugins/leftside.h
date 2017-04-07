#pragma once
#include <QtPlugin>
#include "includes/plugin.h"

namespace qnoto {

FORWARD(LeftSide)

class COMMON_EXPORT LeftSide: public Plugin
{
    Q_OBJECT
public:
    virtual QWidget* create() const = 0;
};

}

Q_DECLARE_INTERFACE(qnoto::LeftSide, "qNoto.LeftSide")
