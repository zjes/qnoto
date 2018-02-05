#include "formater.h"
#include "syntaxhighlighter.h"

Formater::~Formater()
{
    stop();
}

void Formater::stop()
{
    m_mut.lock();
    m_runned = false;
    m_wait.wakeOne();
    m_mut.unlock();

    wait();
}

void Formater::add(const TextBlock& blk)
{
    QMutexLocker lock(&m_mut);
    m_queue.enqueue(blk);
    m_wait.wakeOne();
}

void Formater::run()
{
    m_runned = true;
    while(m_runned){
        m_mut.lock();
        if (m_queue.size() == 0 && m_runned)
            m_wait.wait(&m_mut);
        m_mut.unlock();

        if (!m_runned)
            break;

        if (m_queue.size()){
            m_mut.lock();
            auto it = m_queue.dequeue();
            m_mut.unlock();
            if (m_func(it))
                emit processed(it);
        }
    }
}
