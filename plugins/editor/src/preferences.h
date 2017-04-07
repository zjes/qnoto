#pragma once

#include <QWidget>
#include "includes/plugin.h"

namespace Ui
{ class Preferences; }

class Preferences : public qnoto::PreferencesPage
{
    Q_OBJECT
public:
    explicit Preferences();
    ~Preferences();

    void init();

    bool save() override;
private:
    void changeFont();
private:
    QScopedPointer<Ui::Preferences> m_ui;
};
