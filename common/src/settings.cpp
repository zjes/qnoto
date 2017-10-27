#include <QDebug>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariant>
#include "includes/settings.h"

namespace qnoto {


void joinTree(const QStringList& path, const QJsonValue& val,  QSettings::SettingsMap &map)
{
    if (val.isObject()){
        QJsonObject obj = val.toObject();
        for(const auto& it: obj.keys()){
            QStringList thisPath = path;
            thisPath.append(it);
            joinTree(thisPath, obj[it], map);
        }
    } else if (val.isArray()){
        QVariantList lst;
        for(auto it: val.toArray()){
            lst.append(it.toVariant());
        }
        map.insert(path.join("/"), lst);
    } else {
        map.insert(path.join("/"), val.toVariant());
    }
}

bool readJsonFile(QIODevice &device, QSettings::SettingsMap &map)
{
    QJsonDocument doc = QJsonDocument::fromJson(device.readAll());
    QJsonObject root = doc.object();
    for(const auto& it: root.keys()){
        QStringList path = {it};
        joinTree(path, root[it], map);
    }
    return true;
}

QJsonValue saveVariant(const QVariant& value)
{
    if (value.type() == QVariant::List){
        QJsonArray arr;
        for(const QVariant& val: value.toList()){
            arr.append(saveVariant(val));
        }
        return arr;
    }
    return QJsonValue::fromVariant(value);
}

void write(QJsonObject& parent, QStringList& keys, const QVariant& value)
{
    if (keys.size() == 1){
        parent[keys[0]] = saveVariant(value);
    } else {
        QString key = keys.takeFirst();
        QJsonObject child;
        if (parent.contains(key))
            child = parent[key].toObject();

        write(child, keys, value);
        parent[key] = child;
    }
}

bool writeJsonFile(QIODevice &device, const QSettings::SettingsMap &map)
{
    QJsonObject obj;

    for(const QString& key: map.keys()){
        QStringList skeys = key.split("/");

        if (!map[key].isNull())
            write(obj, skeys, map[key]);
    }

    QJsonDocument doc(obj);
    device.write(doc.toJson());
    return true;
}

Settings::Settings(const QString& name):
    m_settings(new QSettings(QSettings::registerFormat("json", readJsonFile, writeJsonFile), QSettings::UserScope, "qnoto", name))
{}

Settings::~Settings()
{}

void Settings::setValue(const QString& key, const QString& value)
{
    _setValue(key, value);
}

QString Settings::value(const QString& key, const QString& def)
{
    return m_settings->value(key, def).toString();
}

void Settings::_setValue(const QString& key, const QVariant& value)
{
    m_settings->setValue(key, value);
}

QVariant Settings::_value(const QString& key, const QVariant& def)
{
    return m_settings->value(key, def);
}

bool Settings::save()
{ return false; }

PropSignal::PropSignal()
{}

}
