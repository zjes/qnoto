#pragma once
#include <QtPlugin>
#include "includes/plugin.h"

class QMenu;

namespace qnoto {

FORWARD(Editor)

class COMMON_EXPORT EditorInstance: public QWidget
{
    Q_OBJECT
public:
    virtual bool init(const QString& fileName) = 0;
    virtual void populateMenu(QMenu* menu) = 0;
};

class COMMON_EXPORT Editor: public Plugin
{
    Q_OBJECT
public:
    virtual EditorInstance* create() = 0;
};

}

Q_DECLARE_INTERFACE(qnoto::Editor, "qNoto.Editor")

