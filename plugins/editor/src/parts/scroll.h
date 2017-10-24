#pragma once
#include <QList>
#include <QScrollBar>

class QPaintEvent;

class Scroll: public QScrollBar
{
public:
    void setPosses(const QList<int>& posses, int count);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    QList<int> m_posses;
    int m_count;
};
