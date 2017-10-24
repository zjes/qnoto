#pragma once
#include <QListWidget>

class HistoryNavigate : public QListWidget
{
    Q_OBJECT
public:
    HistoryNavigate(QWidget *parent);
    ~HistoryNavigate() override;
public:
    void next();
    void previous();
signals:
    void closed(const QString &);
protected:
    virtual bool event(QEvent *e);
private:
    int m_current = 1;
};

