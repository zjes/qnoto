#include <QDebug>
#include <QVBoxLayout>
#include <QSplitter>
#include <QSettings>

#include "side-split.h"
#include "tool-place.h"
#include "includes/ui-utils.h"

SideSplit::SideSplit(QWidget* parent):
    QWidget(parent),
    m_splitter(new QSplitter(Qt::Vertical, this))
{
    qnoto::setLayout<QVBoxLayout>(this, m_splitter);
}

SideSplit::~SideSplit()
{}

void SideSplit::addSplit(const QString& item)
{
    auto* cont = new ToolPlace(item, this);

    connect(cont, &ToolPlace::addSplit, this, &SideSplit::addSplit);
    connect(cont, &ToolPlace::closeSplit, this, &SideSplit::closeSplit);

    m_splitter->addWidget(cont);
    cont->restoreState();
}

void SideSplit::closeSplit()
{
    if (m_splitter->count() == 1)
        return;

    for(int i = 0; i < m_splitter->count(); ++i){
        if (m_splitter->widget(i) != sender())
            continue;

        m_splitter->widget(i)->deleteLater();
        break;
    }
}

void SideSplit::saveState(QSettings& sets) const
{
    sets.setValue("MainWindow/LeftSplitter", m_splitter->saveState());
    QStringList items;
    for(int i = 0; i < m_splitter->count(); ++i){
        ToolPlace* cont = qobject_cast<ToolPlace*>(m_splitter->widget(i));
        if (!cont)
            continue;
        items.append(cont->selectedPlugin());
    }
    sets.setValue("MainWindow/LeftItems", items);
}

void SideSplit::restoreState(const QSettings& sets)
{
    QStringList items = sets.value("MainWindow/LeftItems").toStringList();
    if (!items.size()){
        addSplit("file-browser");
        addSplit("opened-files");
    } else {
        for(const QString& str: items)
            addSplit(str);
    }
    m_splitter->restoreState(sets.value("MainWindow/LeftSplitter").toByteArray());
}

