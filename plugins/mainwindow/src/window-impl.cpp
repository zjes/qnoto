#include <QDebug>
#include <QDockWidget>
#include <QSettings>
#include <QCloseEvent>
#include "window-impl.h"
#include "ui_window-impl.h"
#include "side-split.h"
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

    QDockWidget *dockWidget = new QDockWidget("Left side", this);
    dockWidget->setContentsMargins(0, 0, 0, 0);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
    dockWidget->setFloating(false);
    dockWidget->setFeatures(dockWidget->features() ^ QDockWidget::DockWidgetClosable ^ QDockWidget::DockWidgetMovable ^ QDockWidget::DockWidgetFloatable);
    dockWidget->setTitleBarWidget(new QWidget);
    m_split = new SideSplit(this);
    dockWidget->setWidget(m_split);
    addDockWidget(Qt::LeftDockWidgetArea, dockWidget);

    m_editors = qnoto::plugin<qnoto::Editors>();
    connect(m_editors, &qnoto::Editors::updateEditMenu, this, &WindowImpl::updateEditMenu);

    setCentralWidget(m_editors);
    m_split->restoreState(sets);
    m_editors->restoreState();

    const auto& acts = m_editors->actions();
    for(const auto& it: acts){
        if (it->property("name").toString().startsWith("File_")){
            m_ui->menuFile->addAction(it);
        }
    }

    connect(m_ui->actionPreferences, &QAction::triggered, this, &WindowImpl::showPreferences);
}

WindowImpl::~WindowImpl()
{
}

void WindowImpl::updateEditMenu(const QList<QAction*>& actions)
{
    m_ui->menuEdit->clear();
    for(const auto& it: actions){
        if (it->property("name").toString().startsWith("Edit_")){
            m_ui->menuEdit->addAction(it);
        }
    }
}

bool WindowImpl::openEditor(const QString& file)
{
    m_editors->openFile(file);
    return false;
}

void WindowImpl::closeEvent(QCloseEvent* event)
{
    emit beforeClose();
    QSettings sets(QSettings::IniFormat, QSettings::UserScope, "qnoto", "qnoto");

    sets.setValue("MainWindow/Geometry", saveGeometry());

    m_split->saveState(sets);
    if (!m_editors->saveState())
        event->ignore();
}

void WindowImpl::showPreferences()
{
    Preferences prefs;
    if (prefs.exec() == QDialog::Accepted){
        prefs.save();
    }
}
