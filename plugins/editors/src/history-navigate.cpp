#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QFileInfo>
#include "history-navigate.h"
#include "includes/file-handler.h"

HistoryNavigate::HistoryNavigate(QWidget *parent):
    QListWidget(parent)
{
    setWindowFlags(Qt::Widget | Qt::Popup);
    setAttribute(Qt::WA_DeleteOnClose);

    for(const QString& file: qnoto::FileHandler::instance().openedFiles()){
        QFileInfo inf(file);
        QString name = inf.fileName();
        QListWidgetItem *it = new QListWidgetItem(name, this);
        it->setData(Qt::UserRole, file);
    }
    if (m_current < count()) {
        setCurrentRow(m_current);
    } else {
        m_current = count()-1;
        setCurrentRow(m_current);
    }
    show();
}

HistoryNavigate::~HistoryNavigate()
{
}

void HistoryNavigate::next()
{
    m_current++;
    if (m_current >= count())
        m_current = 0;

    setCurrentRow(m_current);
}

void HistoryNavigate::previous()
{

}

bool HistoryNavigate::event(QEvent *event)
{
    if (event->type() == QEvent::Show){
        QPoint p = parentWidget()->mapToGlobal(parentWidget()->rect().topLeft());
        move(p.x() + (parentWidget()->geometry().width()-width())/2, p.y() + (parentWidget()->geometry().height()-height())/2);
        setFocus();
        return false;
    }

    if (event->type() == QEvent::KeyRelease && (static_cast<QKeyEvent*>(event))->key() == Qt::Key_Control){
        close();
        return false;
    }

    if (event->type() == QEvent::KeyPress && (static_cast<QKeyEvent*>(event))->key() == Qt::Key_Tab && (static_cast<QKeyEvent*>(event))->modifiers() & Qt::ControlModifier){
        next();
        return false;
    }

    if (event->type() == QEvent::Close){
        if (currentItem())
            emit closed(currentItem()->data(Qt::UserRole).toString());
    }

    return QListWidget::event(event);
}
