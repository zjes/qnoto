#pragma once
#include "includes/plugins/mainwindow.h"
#include "window-impl.h"

class MainWindow: public qnoto::MainWindow
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "qNoto.MainWindow")
    Q_INTERFACES(qnoto::MainWindow)
public:
    void show() override;
    void openFiles(const QStringList& files) override;
    QString name() const override;
    QString title() const override;
public:
    MainWindow();
    ~MainWindow();
private:
    WindowImpl* window();
    void beforeClose();
private:
    QSharedPointer<WindowImpl> m_window;
};

