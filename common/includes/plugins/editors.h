#pragma once
#include <QtPlugin>
#include "includes/plugin.h"

class QMenu;

namespace qnoto {

FORWARD(Editors)

class COMMON_EXPORT Editors: public Plugin
{
    Q_OBJECT
public:
    virtual void openFile(const QString& file) = 0;
    virtual void setMenu(QMenu* menu) = 0;
};

}

Q_DECLARE_INTERFACE(qnoto::Editors, "qNoto.Editors")
