#pragma once
#include <QWidget>

class EditorImpl;

class EditorMargin: public QWidget
{
public:
    EditorMargin(EditorImpl* editor);
public:
    int preferedWidth() const;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent * event) override;
private:
    EditorImpl* m_editor;
};

