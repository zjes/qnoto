#pragma once
#include <QWidget>

class QSplitter;
class QSettings;

class SideSplit: public QWidget
{
    Q_OBJECT
public:
    SideSplit(QWidget* parent = nullptr);
    ~SideSplit() override;

    void saveState(QSettings& sets) const;
    void restoreState(const QSettings& sets);

private:
    void addSplit(const QString& item = "FileBrowser");
    void closeSplit();

private:
    QSplitter* m_splitter;
};
