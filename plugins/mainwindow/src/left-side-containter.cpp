#include <QDebug>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolButton>
#include <QStackedWidget>
#include <QSizePolicy>
#include <QComboBox>
#include "left-side-containter.h"
#include "includes/pluginmanager.h"

Header::Header(const QList<qnoto::LeftSidePtr>& plugins, const QString& selected, QWidget* parent):
    QWidget(parent)
{
    setLayout(new QHBoxLayout(this));
    layout()->setContentsMargins(1, 1, 1, 1);
    layout()->setSpacing(0);
    setProperty("panelWidget", true);
    setMaximumHeight(22);

    m_box = new QComboBox;
    QToolButton *split = new QToolButton;
    QToolButton *close = new QToolButton;

    split->setIcon(QIcon(":/icons/split-horizontal.png"));
    close->setIcon(QIcon(":/icons/close.png"));

    connect(close, &QToolButton::clicked, [this](bool){
        emit closeSplit();
    });
    connect(split, &QToolButton::clicked, [this](bool){
        emit addSplit(selectedPluginName());
    });
    connect(m_box, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, [this](int){
        emit showPlugin(selectedPluginName());
    });

    for (const auto& plug: plugins){
        m_box->addItem(plug->title(), plug->name());
    }
    m_box->setCurrentIndex(m_box->findData(selected));

    layout()->addWidget(m_box);
    layout()->addWidget(split);
    layout()->addWidget(close);

    QTimer::singleShot(10, [this, selected](){
        emit showPlugin(selected);
    });
}

void Header::addItem(qnoto::LeftSide* plugin)
{
    m_box->addItem(plugin->title(), plugin->name());
}

QString Header::selectedPluginName() const
{
    return m_box->currentData().toString();
}

//--------------------------------------------------------------------------------------------------

LeftSideContainter::LeftSideContainter(const QString& selected, QWidget *parent):
    QWidget(parent)
{
    collectPlugins();

    setLayout(new QVBoxLayout(this));
    layout()->setContentsMargins(0, 0, 0, 0);
    layout()->setSpacing(0);

    m_stack = new QStackedWidget(this);
    m_stack->layout()->setContentsMargins(0, 0, 0, 0);
    m_stack->layout()->setSpacing(0);

    m_header = new Header(m_plugins, selected, this);

    layout()->addWidget(m_header);
    layout()->addWidget(m_stack);

    connect(m_header, &Header::addSplit,   this, &LeftSideContainter::addSplit);
    connect(m_header, &Header::closeSplit, this, &LeftSideContainter::closeSplit);
    connect(m_header, &Header::showPlugin, this, &LeftSideContainter::showPlugin);
}

void LeftSideContainter::collectPlugins()
{
    for(qnoto::Plugin* pl: qnoto::plugins<qnoto::LeftSide>()){
        auto* ls = qobject_cast<qnoto::LeftSide*>(pl);
        if (ls)
            m_plugins.append(qnoto::LeftSidePtr(ls));
    }
}

void LeftSideContainter::showPlugin(const QString & name)
{
    if (m_stackIndex.contains(name)){
        m_stack->setCurrentIndex(m_stackIndex[name]);
    } else {
        for(const qnoto::LeftSidePtr& plug: m_plugins){
            if (plug->name() != name)
                continue;
            m_stackIndex.insert(name, m_stack->addWidget(plug->create()));
            m_stack->setCurrentIndex(m_stackIndex[name]);
            break;
        }
    }
}

QString LeftSideContainter::selectedPlugin() const
{
    return m_header->selectedPluginName();
}

