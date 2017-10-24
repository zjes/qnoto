#pragma once
#include <QMainWindow>
#include <QScopedPointer>
#include "includes/plugins/editors.h"

namespace Ui
{ class WindowImpl; }

class SideSplit;

class WindowImpl: public QMainWindow
{
    Q_OBJECT
public:
    WindowImpl();
    ~WindowImpl() override;

    bool openEditor(const QString& file);
protected:
    void closeEvent(QCloseEvent* event) override;
private:
    void showPreferences();
    void updateEditMenu(const QList<QAction*>& actions);
signals:
    void beforeClose();
private:
    QScopedPointer<Ui::WindowImpl> m_ui;
    qnoto::Editors* m_editors;
    SideSplit* m_split;
};
