#include <QSettings>
#include <QCoreApplication>
#include <QFont>
#include <QDebug>
#include <QFontDatabase>
#include "settings.h"

Settings::Settings():
    m_setts(new QSettings(QSettings::IniFormat, QSettings::UserScope, "qnoto", "qnoto"))
{}

Settings& Settings::instance()
{
    static Settings sets;
    return sets;
}

Settings::~Settings()
{}

QFont Settings::font()
{
    QFont font;
    font.setBold(instance().m_setts->value("editor/font-bold", false).toBool());
    font.setItalic(instance().m_setts->value("editor/font-italic", false).toBool());
    if (instance().m_setts->value("editor/font-family").isNull()){
        QFont sys = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setFamily(sys.family());
        font.setPointSize(sys.pointSize());
    } else {
        font.setFamily(instance().m_setts->value("editor/font-family").toString());
        font.setPointSize(instance().m_setts->value("editor/font-size", 11).toInt());
    }
    return font;
}

void Settings::setFont(const QFont& font)
{
    instance().m_setts->setValue("editor/font-bold", font.bold());
    instance().m_setts->setValue("editor/font-italic", font.italic());
    instance().m_setts->setValue("editor/font-family", font.family());
    instance().m_setts->setValue("editor/font-size", font.pointSize());
}

bool Settings::wordWrap()
{
    return instance().m_setts->value("editor/wordwrap", false).toBool();
}

QString Settings::theme()
{
    return instance().m_setts->value("editor/theme", QCoreApplication::applicationDirPath()+"/themes/breeze-dark.theme").toString();
}

bool Settings::showLineNumbers()
{
    return instance().m_setts->value("editor/show-linenumbers", true).toBool();
}

bool Settings::showWhite()
{
    return instance().m_setts->value("editor/show-white", true).toBool();
}

bool Settings::showEndl()
{
    return instance().m_setts->value("editor/show-endl", false).toBool();
}
