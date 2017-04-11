#pragma once

#include <QWidget>

class EditorImpl;

namespace Ui
{ class EditorFind; }

class EditorFind : public QWidget
{
    Q_OBJECT
public:
    EditorFind(EditorImpl *parent);
    ~EditorFind();
public:
    void activate();
protected:
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
private:
    EditorImpl* m_editor;
    QScopedPointer<Ui::EditorFind> m_ui;
};
