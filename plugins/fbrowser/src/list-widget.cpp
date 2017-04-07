#include <QDebug>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QTimer>
#include "includes/file-handler.h"
#include "list-widget.h"
#include "list-model.h"

ListWidget::ListWidget(QWidget *parent) :
    QTreeView(parent)
{
    qnoto::FileHandler& fh = qnoto::FileHandler::instance();
    connect(&fh, &qnoto::FileHandler::activated, this, &ListWidget::fileActivated);

    connect(this, &ListWidget::activated, [this](const QModelIndex & index){
        QFileInfo file = qobject_cast<ListModel*>(model())->fileInfo(index);
        if (file.isFile())
            emit qnoto::FileHandler::instance().activated(file.absoluteFilePath());
    });

    this->header()->setVisible(false);
    QTimer::singleShot(100, this, &ListWidget::init);
}

void ListWidget::init()
{
    auto *lstmodel = new ListModel();
    setSortingEnabled(true);
    sortByColumn(0, Qt::AscendingOrder);
    setModel(lstmodel);
}

void ListWidget::fileActivated(const QString& /*file*/)
{
}
