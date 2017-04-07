#pragma once
#include <QWidget>

class EditorImpl;

class EditorHeader: public QWidget
{
    Q_OBJECT
public:
    EditorHeader(EditorImpl* editor);
private:
    EditorImpl* m_editor;
};
