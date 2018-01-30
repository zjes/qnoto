#pragma once
#include <QString>
#include <QVariant>
#include "convert.h"
#include "common-export.h"

class QSettings;

namespace qnoto {

class Settings;

class COMMON_EXPORT PropSignal: public QObject
{
    Q_OBJECT
public:
    PropSignal();
signals:
    void changed();
};

template<typename T>
class Property
{
public:
    Property(Settings& sets, const QString& path, const T& defValue):
        m_settings(sets),
        m_path(path),
        m_default(defValue)
    {}

    Property& operator=(const T& value);
    operator T() const;
    PropSignal signal;
private:
    Settings& m_settings;
    QString m_path;
    T m_default;
};

class COMMON_EXPORT Settings: public QObject
{
    Q_OBJECT
public:
    virtual ~Settings();
    bool save();

public:
    template<typename T, typename = std::enable_if_t<!std::is_same<std::decay_t<T>, QString>{}>>
    void setValue(const QString& key, const T& value)
    {
        _setValue(key, QVariant::fromValue<T>(value));
    }

    template<typename T>
    std::enable_if_t<!std::is_same<T, std::decay_t<QString>>::value, T>
    value(const QString& key, const T& def = {}) const
    {
        return _value(key, def).template value<T>();
    }

    void setValue(const QString& key, const QString& value);
    QString value(const QString& key, const QString& def = {}) const;

protected:
    Settings(const QString& name);

private:
    void _setValue(const QString& key, const QVariant& value);
    QVariant _value(const QString& key, const QVariant& def) const;

private:
    QScopedPointer<QSettings> m_settings;
};

template<typename T>
Property<T>& Property<T>::operator=(const T& value)
{
    if (value != m_settings.value(m_path, m_default)){
        m_settings.setValue(m_path, value);
        emit signal.changed();
    }
    return *this;
}

template<typename T>
Property<T>::operator T() const
{
    return m_settings.value(m_path, m_default);
}

}
