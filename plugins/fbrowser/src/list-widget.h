#pragma once

#include <QTreeView>

class FileListModel;

class ListWidget : public QTreeView
{
    Q_OBJECT
public:
    explicit ListWidget(QWidget *parent = 0);
    void init();
private:
    void fileActivated(const QString& file);
signals:
    void fileSelected(const QString & file);
};
