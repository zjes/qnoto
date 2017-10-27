#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QFileInfo>
#include <QAction>
#include <QApplication>
#include <QShortcut>
#include "switcher.h"
#include "includes/file-handler.h"

Switcher::Switcher(QWidget *parent):
    QListWidget(parent)
{
    setWindowFlags(Qt::Widget | Qt::Popup);
    setContentsMargins(5, 5, 5, 5);
}

Switcher::~Switcher()
{
}

void Switcher::setForwardAction(QAction* action)
{
    m_forwardKey = new QShortcut(action->shortcut(), this);
    connect(action, &QAction::triggered, [this]{
        run(true);
    });
    connect(m_forwardKey, &QShortcut::activated, [this]{
        run(true);
    });
}

void Switcher::setBackwardAction(QAction* action)
{
    m_backwardKey = new QShortcut(action->shortcut(), this);
    connect(action, &QAction::triggered, [this]{
        run(false);
    });
    connect(m_backwardKey, &QShortcut::activated, [this]{
        run(false);
    });
}

void Switcher::run(bool forward)
{
    if (isVisible())
        selectItem(forward);
    else
        showSwitcher(forward);
}

void Switcher::showSwitcher(bool forward)
{
    m_current = 0;
    clear();

    for(const QString& file: qnoto::FileHandler::instance().openedFiles()){
        QFileInfo inf(file);
        QString name = inf.fileName();
        QListWidgetItem *it = new QListWidgetItem(name, this);
        it->setData(Qt::UserRole, file);
    }

    selectItem(forward);

    if (QApplication::queryKeyboardModifiers() != 0){
        QPoint p = parentWidget()->mapToGlobal(parentWidget()->rect().topLeft());
        move(p.x() + (parentWidget()->geometry().width()-width())/2, p.y() + (parentWidget()->geometry().height()-height())/2);
        setFocus();
        show();
    } else {
        qnoto::FileHandler::instance().activate(currentItem()->data(Qt::UserRole).toString());
    }
}

void Switcher::selectItem(bool forward)
{
    m_current += forward ? 1 : -1;
    if(m_current >= count())
        m_current = 0;
    if(m_current < 0)
        m_current = count()-1;

    setCurrentRow(m_current);
}

void Switcher::keyReleaseEvent(QKeyEvent *event)
{
    if (event->modifiers() == 0){
        close();
        qnoto::FileHandler::instance().activate(currentItem()->data(Qt::UserRole).toString());
    }
    QWidget::keyReleaseEvent(event);
}


