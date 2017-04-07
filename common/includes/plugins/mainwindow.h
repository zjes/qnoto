#pragma once
#include <QtPlugin>
#include "includes/plugin.h"

namespace qnoto {

FORWARD(MainWindow)

class COMMON_EXPORT MainWindow: public Plugin
{
    Q_OBJECT
public:
    virtual void openFiles(const QStringList& files) = 0;
    virtual void show() = 0;
};

}

Q_DECLARE_INTERFACE(qnoto::MainWindow, "qNoto.MainWindow")
