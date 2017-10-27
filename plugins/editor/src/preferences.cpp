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
    Settings::instance().showLineNumbers = m_ui->showLineNumber->isChecked();
    return true;
}

void Preferences::init()
{
    QFont font = Settings::instance().font();
    m_ui->fontShow->setFont(font);
    m_ui->fontShow->setText(QString("%1 %2pt").arg(font.family()).arg(font.pointSize()));
    m_ui->antiAlias->setChecked(Settings::instance().antialiasedFont);
    m_ui->whiteSpace->setChecked(Settings::instance().showWhite);
    m_ui->showEndl->setChecked(Settings::instance().showEndl);
    m_ui->showLineNumber->setChecked(Settings::instance().showLineNumbers);
    m_ui->hightLine->setChecked(Settings::instance().hightlightLine);
}

void Preferences::changeFont()
{
    QFontDialog dial;
    dial.setCurrentFont(Settings::instance().font());
    if (dial.exec() == QDialog::Accepted){
        QFont font = dial.currentFont();
        Settings::instance().setFont(font);
        m_ui->fontShow->setText(QString("%1 %2pt").arg(font.family()).arg(font.pointSize()));
    }
}
