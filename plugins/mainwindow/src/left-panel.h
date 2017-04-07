#pragma once
#include <QWidget>

class QSplitter;
class QSettings;

class LeftPanel: public QWidget
{
    Q_OBJECT
public:
    ~LeftPanel();
    explicit LeftPanel(QWidget* parent = nullptr);

    void saveState(QSettings& sets) const;
    void restoreState(const QSettings& sets);

private:
    void addSplit(const QString& item = "FileBrowser");
    void closeSplit();

private:
    QSplitter* m_splitter;
};
