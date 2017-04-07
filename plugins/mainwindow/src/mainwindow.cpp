#include <QDebug>
#include <QResource>
#include "mainwindow.h"

MainWindow::MainWindow()
{
    QResource::registerResource("resources.rcc");
}

MainWindow::~MainWindow()
{
    QResource::unregisterResource("resources.rcc");
}

WindowImpl* MainWindow::window()
{
    if (!m_window){
        m_window = QSharedPointer<WindowImpl>(new WindowImpl, [](WindowImpl* w){
            w->deleteLater();
        });
        connect(m_window.data(), &WindowImpl::beforeClose, this, &MainWindow::beforeClose);
    }
    return m_window.data();
}

void MainWindow::show()
{
    window()->show();
}

void MainWindow::openFiles(const QStringList& files)
{
    for(const QString& file: files)
        window()->openEditor(file);
}

void MainWindow::beforeClose()
{
    qDebug() << "before close";
}

QString MainWindow::name() const
{
    return "main-window";
}

QString MainWindow::title() const
{
    return tr("Main window");
}

