#pragma once
#include <QObject>

class Editors;
class EditorsState: public QObject
{
    Q_OBJECT
public:
    EditorsState(Editors* eds);
private:
    void modified(const QString& fileName, bool modified);
    void activated(const QString& fileName);
    void closed(const QString& fileName);
    void openedChanged();
private:
    Editors* m_eds;
};
