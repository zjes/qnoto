#pragma once
#include <QMainWindow>
#include <QScopedPointer>
#include "includes/plugins/editors.h"

namespace Ui
{ class WindowImpl; }

class LeftPanel;

class WindowImpl: public QMainWindow
{
    Q_OBJECT
public:
    WindowImpl();
    ~WindowImpl();

    bool openEditor(const QString& file);
protected:
    void closeEvent(QCloseEvent* event) override;
private:
    void showPreferences();
signals:
    void beforeClose();
private:
    QScopedPointer<Ui::WindowImpl> m_ui;
    qnoto::Editors* m_editors;
    LeftPanel* m_panel;
};
