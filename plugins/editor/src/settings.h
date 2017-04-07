#pragma once
#include <QScopedPointer>

class QSettings;
class QFont;

class Settings
{
public:
    static Settings& instance();
    ~Settings();
public:
    static QFont font();
    static void setFont(const QFont& font);
    static bool wordWrap();
    static QString theme();
    static bool showLineNumbers();
    static bool showWhite();
    static bool showEndl();
private:
    Settings();
private:
    QScopedPointer<QSettings> m_setts;
};
