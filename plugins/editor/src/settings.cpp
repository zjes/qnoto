#include <QSettings>
#include <QCoreApplication>
#include <QFont>
#include <QDebug>
#include <QFontDatabase>
#include "settings.h"

Settings::Settings():
    qnoto::Settings("editor"),
    wordWrap(*this,         "editor/wordwrap",          false),
    theme(*this,            "editor/theme",             QCoreApplication::applicationDirPath()+"/themes/breeze-dark.theme"),
    showLineNumbers(*this,  "editor/show-linenumbers",  true),
    showWhite(*this,        "editor/show-white",        true),
    showEndl(*this,         "editor/show-endl",         false),
    antialiasedFont(*this,  "editor/antialiased-font",  true),
    hightlightLine(*this,   "editor/hightlight-line",   false)
{}

Settings& Settings::instance()
{
    static Settings sets;
    return sets;
}

QFont Settings::font()
{
    QFont font;
    font.setBold(value("editor/font-bold", false));
    font.setItalic(value("editor/font-italic", false));

    if (value("editor/font-family").isNull()){
        QFont sys = QFontDatabase::systemFont(QFontDatabase::FixedFont);
        font.setFamily(sys.family());
        font.setPointSize(sys.pointSize());
    } else {
        font.setFamily(value("editor/font-family"));
        font.setPointSize(value("editor/font-size", 11));
    }

    font.setStyleStrategy(antialiasedFont ? QFont::PreferAntialias : QFont::NoAntialias);
    return font;
}

void Settings::setFont(const QFont& font)
{
    instance().setValue("editor/font-bold", font.bold());
    instance().setValue("editor/font-italic", font.italic());
    instance().setValue("editor/font-family", font.family());
    instance().setValue("editor/font-size", font.pointSize());
}
