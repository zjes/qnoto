#include <QDebug>
#include <QDockWidget>
#include <QSettings>
#include "window-impl.h"
#include "ui_window-impl.h"
#include "left-panel.h"
#include "qnoto-style.h"
#include "preferences.h"
#include "includes/pluginmanager.h"

WindowImpl::WindowImpl():
    m_ui(new Ui::WindowImpl)
{
    m_ui->setupUi(this);
    QApplication::setStyle(new QNotoStyle);

    QSettings sets(QSettings::IniFormat, QSettings::UserScope, "qnoto", "qnoto");
    restoreGeometry(sets.value("MainWindow/Geometry").toByteArray());

    QDockWidget *dockWidget = new QDockWidget("Left side");
    dockWidget->setContentsMargins(0, 0, 0, 0);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    dockWidget->setFloating(false);
    dockWidget->setFeatures(dockWidget->features() ^ QDockWidget::DockWidgetClosable ^ QDockWidget::DockWidgetMovable ^ QDockWidget::DockWidgetFloatable);
    dockWidget->setTitleBarWidget(new QWidget);
    m_panel = new LeftPanel;
    dockWidget->setWidget(m_panel);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

    m_editors = qnoto::plugin<qnoto::Editors>();
    m_editors->setMenu(m_ui->menuEdit);
    setCentralWidget(m_editors);
    m_panel->restoreState(sets);
    m_editors->restoreState(sets);

    connect(m_ui->actionPreferences, &QAction::triggered, this, &WindowImpl::showPreferences);
}

WindowImpl::~WindowImpl()
{
}

bool WindowImpl::openEditor(const QString& file)
{
    m_editors->openFile(file);
    return false;
}

void WindowImpl::closeEvent(QCloseEvent* /*event*/)
{
    emit beforeClose();
    QSettings sets(QSettings::IniFormat, QSettings::UserScope, "qnoto", "qnoto");

    sets.setValue("MainWindow/Geometry", saveGeometry());

    m_panel->saveState(sets);
    m_editors->saveState(sets);
}

void WindowImpl::showPreferences()
{
    Preferences prefs;
    if (prefs.exec() == QDialog::Accepted){
        prefs.save();
    }
}
