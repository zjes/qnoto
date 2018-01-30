#pragma once
#include <QQmlComponent>
#include "common-export.h"

class QSettings;

namespace qnoto {

class COMMON_EXPORT Component: public QQmlComponent
{
    Q_OBJECT
public:
    Component(QQmlEngine* engine, const QString& file, QObject* parent);
    ~Component() override;
    void setContext(const QString& name, QObject* context);
private:
    QObject* m_context = nullptr;
};

class COMMON_EXPORT Plugin: public QObject
{
    Q_OBJECT
public:
    Plugin();
    virtual ~Plugin();
    virtual QString name() const = 0;
    virtual QString title() const = 0;
    virtual QObject* create(QObject* parent) = 0;

public:
    static Component* createComponent(QObject* parent, const QString& fileName);
};

}

Q_DECLARE_INTERFACE(qnoto::Plugin, "qNoto.Plugin")
