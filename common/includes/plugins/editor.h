#pragma once
#include <QtPlugin>
#include "includes/plugin.h"
#include "includes/plugins/editors.h"

class QMenu;

namespace qnoto {

FORWARD(Editor)

class COMMON_EXPORT EditorInstance: public QWidget
{
    Q_OBJECT
public:
    virtual bool init(const QString& fileName) = 0;
    virtual bool isModified() const = 0;
    virtual const QString& fileName() const = 0;
    virtual bool save(const QString& newFileName = {}) = 0;
    virtual const QList<QAction*>& actions() = 0;
public:
    virtual void setSearch(const QString& text = {}) = 0;
    virtual QString selectedText() const = 0;
    virtual void doSearch() = 0;

signals:
    void escape();
};

class COMMON_EXPORT Editor: public Plugin
{
    Q_OBJECT
public:
    virtual EditorInstance* create() = 0;
};

}

Q_DECLARE_INTERFACE(qnoto::Editor, "qNoto.Editor")

