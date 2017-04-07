#include <QFileInfo>
#include <QListWidget>
#include <QVBoxLayout>
#include "opened-files.h"
#include "includes/file-handler.h"

class ListWidget: public QListWidget
{
    Q_OBJECT
public:
    ListWidget()
    {
        qnoto::FileHandler& fh = qnoto::FileHandler::instance();
        connect(&fh, &qnoto::FileHandler::activated, this, &ListWidget::fileActivated);
        connect(&fh, &qnoto::FileHandler::closed, this, &ListWidget::fileClosed);
        connect(&fh, &qnoto::FileHandler::modified, this, &ListWidget::fileModified);

        for(const QString& file: fh.openedFiles())
            fileActivated(file);

        connect(this, &ListWidget::itemClicked, [](QListWidgetItem* item){
            qnoto::FileHandler::instance().activated(item->data(Qt::UserRole+1).toString());
        });
    }

private:
    void fileActivated(const QString& file)
    {
        auto*item = byFileName(file);
        if (item){
            setCurrentItem(item);
        } else {
            QFileInfo info(file);
            QListWidgetItem* it = new QListWidgetItem(info.fileName());
            it->setData(Qt::UserRole+1, file);
            it->setData(Qt::UserRole+2, info.fileName());
            addItem(it);
        }
    }

    void fileClosed(const QString& file)
    {
        auto*item = byFileName(file);
        if (!item)
            return;
        removeItemWidget(item);
    }

    void fileModified(const QString& file, bool modified)
    {
        auto*item = byFileName(file);
        if (!item)
            return;
        item->setText(item->data(Qt::UserRole+2).toString()+(modified ? "*" : ""));
    }
private:
    QListWidgetItem* byFileName(const QString& file) const
    {
        for(int i = 0; i < count(); ++i){
            if (item(i)->data(Qt::UserRole+1).toString() != file)
                continue;
            return item(i);
        }
        return nullptr;
    }
};

QString OpenedFiles::name() const
{
    return "opened-files";
}

QString OpenedFiles::title() const
{
    return tr("Open Files");
}

QWidget* OpenedFiles::create() const
{
    return new ListWidget;
}

#include "opened-files.moc"
