#include <QDebug>
#include <QVBoxLayout>
#include <QSplitter>
#include <QSettings>
#include "left-panel.h"
#include "left-side-containter.h"
#include "includes/ui-utils.h"

LeftPanel::LeftPanel(QWidget* parent):
    QWidget(parent)
{
    m_splitter = new QSplitter(Qt::Vertical, this);
    qnoto::setLayout<QVBoxLayout>(this, m_splitter);
}

LeftPanel::~LeftPanel()
{}

void LeftPanel::addSplit(const QString& item)
{
    auto* cont = new LeftSideContainter(item);
    connect(cont, &LeftSideContainter::addSplit, this, &LeftPanel::addSplit);
    connect(cont, &LeftSideContainter::closeSplit, this, &LeftPanel::closeSplit);

    m_splitter->addWidget(cont);
}

void LeftPanel::closeSplit()
{
    if (m_splitter->count() == 1)
        return;

    for(int i = 0; i < m_splitter->count(); ++i){
        if (m_splitter->widget(i) == sender()){
            m_splitter->widget(i)->deleteLater();
            break;
        }
    }
}

void LeftPanel::saveState(QSettings& sets) const
{
    sets.setValue("MainWindow/LeftSplitter", m_splitter->saveState());
    QStringList items;
    for(int i = 0; i < m_splitter->count(); ++i){
        LeftSideContainter* cont = qobject_cast<LeftSideContainter*>(m_splitter->widget(i));
        if (!cont)
            continue;
        items.append(cont->selectedPlugin());
    }
    sets.setValue("MainWindow/LeftItems", items);
}

void LeftPanel::restoreState(const QSettings& sets)
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

