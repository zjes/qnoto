#pragma once
#include <QList>
#include <QAction>
#include "includes/plugins/editors.h"
#include "switcher.h"

class Editors;

class EditorsActions: public QObject
{
    Q_OBJECT
public:
    EditorsActions(Editors* eds);
public:
    const qnoto::Editors::MenuList& actions() const;
    bool canClose() const;
    void refreshRecent();

public:
    template<typename... T>
    qnoto::Editors::MenuList actions(const T&... names) const
    {
        qnoto::Editors::MenuList acts;
        findActions(acts, names...);
        return acts;
    }

private:
    void findActions(qnoto::Editors::MenuList& ret, const QString& name) const
    {
        if (auto* act = findAction(name))
            ret.append(act);
    }

    template<typename... T>
    void findActions(qnoto::Editors::MenuList& ret, const QString& actName, const T&... names) const
    {
        if (auto* act = findAction(actName))
            ret.append(act);
        findActions(ret, names...);
    }

private slots:
    void newFileSlot();
    void openFileSlot();
    void closeFileSlot();
    void closeAllExceptSlot();
    void closeAllSlot();
    void saveFileSlot();
    void saveAsFileSlot();
    void saveAllSlot();
private:
    int savePrompt(const QString& message) const;
    QString lastDir() const;
    QAction* findAction(const QString& actName) const;
private:
    qnoto::Editors::MenuList m_actions;
    Editors* m_eds;
    Switcher m_switcher;
};
