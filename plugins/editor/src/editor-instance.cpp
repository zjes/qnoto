#include "editor-instance.h"

EditorInstance::EditorInstance():
    QObject()
{}

void EditorInstance::load(const QString& fileName)
{
    m_fileName = fileName;
    emit fileNameChanged();
}

const QString& EditorInstance::_fileName() const
{
    return m_fileName;
}
