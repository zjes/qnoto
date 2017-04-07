#include <QFontDialog>
#include "preferences.h"
#include "settings.h"
#include "ui_preferences.h"

Preferences::Preferences():
    m_ui(new Ui::Preferences)
{
    m_ui->setupUi(this);

    connect(m_ui->changeFont, &QPushButton::clicked, this, &Preferences::changeFont);
    init();
}

Preferences::~Preferences()
{}

bool Preferences::save()
{

    return true;
}

void Preferences::init()
{
    QFont font = Settings::font();
    m_ui->fontShow->setFont(font);
    m_ui->fontShow->setText(QString("%1 %2pt").arg(font.family()).arg(font.pointSize()));
}

void Preferences::changeFont()
{
    QFontDialog dial;
    dial.setCurrentFont(Settings::font());
    if (dial.exec() == QDialog::Accepted){
        QFont font = dial.currentFont();
        Settings::setFont(font);
        m_ui->fontShow->setText(QString("%1 %2pt").arg(font.family()).arg(font.pointSize()));
    }
}
