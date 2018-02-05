#pragma once
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>
#include <functional>

class TextBlock;
class Formater: public QThread
{
    Q_OBJECT
public:
    template<typename Cls, typename Func>
    Formater(Cls* cls, Func func):
        m_func([cls, func](TextBlock& block)
        {
            return (cls->*func)(block);
        })
    {}

    ~Formater();
    void stop();
    void add(const TextBlock& blk);
protected:
    void run() override;
signals:
    void processed(const TextBlock& blk);
private:
    bool m_runned;
    QMutex m_mut;
    QWaitCondition m_wait;
    QQueue<TextBlock> m_queue;
    std::function<bool(TextBlock&)> m_func;
};
