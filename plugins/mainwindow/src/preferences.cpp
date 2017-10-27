#include <QDebug>
#include "includes/pluginmanager.h"
#include "preferences.h"
#include "ui_preferences.h"

Preferences::Preferences():
    m_ui(new Ui::Preferences)
{
    m_ui->setupUi(this);
    for(qnoto::Plugin* pl: qnoto::plugins("qNoto")){
        qnoto::PreferencesPage* pref = pl->preferences();
        if (pref){
            int index = m_ui->prefPlace->addWidget(pref);
            m_ui->plugins->insertItem(index, pl->title());
        }
    }

    //connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &Preferences::acceptPrefs);
}

Preferences::~Preferences()
{}

bool Preferences::save() const
{
    return false;
}
