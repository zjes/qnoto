#pragma once
#include <QListWidget>

class QShortcut;

class Switcher : public QListWidget
{
    Q_OBJECT
public:
    Switcher(QWidget *parent);
    ~Switcher() override;
public:
    void setForwardAction(QAction* action);
    void setBackwardAction(QAction* action);

    void showSwitcher(bool forward);
    void selectItem(bool forward);
    void run(bool forward);
protected:
    void keyReleaseEvent(QKeyEvent *event) override;
private:
    int m_current = 1;
    QShortcut* m_forwardKey = nullptr;
    QShortcut* m_backwardKey = nullptr;
};

