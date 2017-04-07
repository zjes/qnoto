#pragma once
#include <QDialog>

namespace Ui
{ class Preferences; }

class Preferences: public QDialog
{
    Q_OBJECT
public:
    Preferences();
    ~Preferences();
    bool save() const;
private:
    QScopedPointer<Ui::Preferences> m_ui;
};
