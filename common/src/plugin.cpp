#include <QQmlContext>
#include <QQmlEngine>
#include "includes/plugin.h"

namespace qnoto {

Plugin::Plugin()
{}

Plugin::~Plugin()
{}

Component* Plugin::createComponent(QObject* parent, const QString& fileName)
{
    auto* engine = qmlContext(parent)->engine();
    qnoto::Component *component = new qnoto::Component(engine, fileName, parent);
    QQmlContext* context = new QQmlContext(engine);
    //component->setParent(parent);
    engine->setContextForObject(component, context);
    return component;
}

Component::Component(QQmlEngine* engine, const QString& file, QObject* parent):
    QQmlComponent(engine, file, parent)
{}

Component::~Component()
{
    delete m_context;
}

void Component::setContext(const QString& name, QObject* handler)
{
    QQmlContext* context = qmlContext(this);
    context->setContextProperty(name, handler);

    m_context = handler;
}

}
