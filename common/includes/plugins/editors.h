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
    using MenuList = QList<QAction*>;
public:
    virtual void openFile(const QString& fileName) = 0;
    virtual void closeFile(const QString& fileName) = 0;
    virtual const MenuList& actions() const = 0;
signals:
    void updateEditMenu(const MenuList& actions);
};

}

Q_DECLARE_INTERFACE(qnoto::Editors, "qNoto.Editors")
