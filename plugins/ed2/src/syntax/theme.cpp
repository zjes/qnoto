#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextFormat>
#include <QMutex>
#include "theme.h"

namespace syntax {

Theme::Theme()
{}

bool Theme::load(const QString& file)
{
    QFile in(file);
    if (!in.exists())
        return false;

    if (!in.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QJsonDocument doc = QJsonDocument::fromJson(in.readAll());
    QJsonObject root = doc.object();
    QJsonObject styles = root["text-styles"].toObject();

    for(const QString& key: styles.keys()){
        QJsonObject style = styles[key].toObject();
        Style st;

        st.color         = style.value("text-color").toString();
        st.backColor     = style.value("back-color").toString();
        st.bold          = style.value("bold").toBool();
        st.italic        = style.value("italic").toBool();
        st.underline     = style.value("underline").toBool();
        st.strikeThrough = style.value("strike-through").toBool();
        st.borderRadius  = style.value("border-radius").toInt();

        m_txtStyles.insert(key, qMove(st));
    }

    QJsonObject stylenums = root["text-stylenums"].toObject();
    for(const QString& key: stylenums.keys()){
        QJsonObject style = stylenums[key].toObject();
        Style st;

        st.color         = style.value("text-color").toString();
        st.backColor     = style.value("back-color").toString();
        st.bold          = style.value("bold").toBool();
        st.italic        = style.value("italic").toBool();
        st.underline     = style.value("underline").toBool();
        st.strikeThrough = style.value("strike-through").toBool();
        st.borderRadius  = style.value("border-radius").toString().toInt();

        m_txtStyleNums.insert(key, qMove(st));
    }


    QJsonObject defaults = root["editor-colors"].toObject();
    for(const QString& key: defaults.keys()){
        m_default.insert(key, QColor(defaults.value(key).toString()));
    }

    return true;
}

void Theme::format(QTextCharFormat& frm, const QString& name, const QString& style) const
{
    auto doFormat = [](QTextCharFormat& frm, const Style& style)
    {
        if (style.color.isValid())
            frm.setForeground(style.color);
        if (style.backColor.isValid())
            frm.setBackground(style.backColor);

        frm.setFontWeight(style.bold ? QFont::Bold: QFont::Normal);
        frm.setFontItalic(style.italic);
        frm.setFontUnderline(style.underline);
        frm.setFontStrikeOut(style.strikeThrough);
        frm.setProperty(QTextFormat::UserProperty+1, style.borderRadius);
        //qDebug() << "rad" << style.borderRadius << frm.property(QTextFormat::UserProperty+1);
    };

    frm.setForeground(m_default.value("text-color"));

    if (!style.isEmpty() && m_txtStyleNums.contains(style))
        doFormat(frm, m_txtStyleNums[style]);

    if (!name.isEmpty() && m_txtStyles.contains(name))
        doFormat(frm, m_txtStyles[name]);
}

QColor Theme::color() const
{
    return m_default.value("text-color");
}

QColor Theme::backgroud() const
{
    return m_default.value("background-color");
}

}
