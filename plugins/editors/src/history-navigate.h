#pragma once
#include <QListWidget>

class HistoryNavigate : public QListWidget
{
    Q_OBJECT
public:
    explicit HistoryNavigate(const QStringList & history, QWidget *parent);
    virtual ~HistoryNavigate();
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

