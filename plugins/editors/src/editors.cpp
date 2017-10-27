#include <QDebug>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QMessageBox>
#include "editors.h"
#include "editors-factory.h"
#include "editors-find.h"
#include "settings.h"

#include "includes/file-handler.h"
#include "includes/ui-utils.h"

//--------------------------------------------------------------------------------------------------

Editors::Editors():
    m_widget(new QStackedWidget),
    m_find(new EditorsFind),
    m_state(this),
    m_actions(this)
{
    qnoto::setLayout<QVBoxLayout>(this, m_widget, m_find);
}

//--------------------------------------------------------------------------------------------------

Editors::~Editors()
{
}

//--------------------------------------------------------------------------------------------------

void Editors::openFile(const QString& file)
{
    QMutexLocker locker(&m_mutex);

    if (m_editors.contains(file)){
        m_widget->setCurrentIndex(m_editors[file].index);
    } else {
        auto* ed = EditorsFactory::create(file);
        connect(ed, &qnoto::EditorInstance::escape, this, &Editors::escape);
        connect(ed, &qnoto::EditorInstance::showFind, this, &Editors::showFind);
        if(ed) {
            int index = m_widget->addWidget(ed);
            m_editors.insert(file, {ed, index});
            m_widget->setCurrentIndex(index);
        }
    }

    m_current = file;
    m_find->setEditor(m_editors[file].editor);
    m_editors[file].editor->setFocus();

    for(QAction* act : actions("File_save", "File_saveAs", "File_close", "File_closeAllExcept")){
        act->setText(act->property("orig-text").toString().arg(QFileInfo(file).fileName()));
    }

    Settings::addToRecent(file);

    m_actions.refreshRecent();
    emit updateEditMenu(m_editors[file].editor->actions());
    emit openedChanged();
}

//--------------------------------------------------------------------------------------------------

void Editors::closeFile(const QString& fileName)
{
    QMutexLocker locker(&m_mutex);

    if (!m_editors.contains(fileName))
        return;

    auto* wid = m_widget->widget(m_editors[fileName].index);
    m_widget->removeWidget(wid);
    m_editors.remove(fileName);
    wid->deleteLater();

    for(int i = 0; i < m_widget->count(); ++i){
        auto* iwid = qobject_cast<qnoto::EditorInstance*>(m_widget->widget(i));
        if (!iwid){
            qWarning() << "wrong wifget type";
            continue;
        }
        m_editors[iwid->fileName()].index = i;
    }

    auto list = Settings::openedFiles();
    list.removeAll(fileName);
    Settings::setOpenedFiles(list);
    emit openedChanged();
}

//--------------------------------------------------------------------------------------------------

void Editors::showFind()
{
    m_find->activate();
}

//--------------------------------------------------------------------------------------------------

void Editors::escape()
{
    if (m_find->isVisible())
        m_find->hide();
}

//--------------------------------------------------------------------------------------------------

bool Editors::saveState() const
{
    if (m_actions.canClose()){
        if (Settings::restoreState())
            Settings::setOpenedFiles(qnoto::FileHandler::instance().openedFiles());
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------------------

void Editors::restoreState()
{
    if (Settings::restoreState()){
        for(const auto& fname: Settings::openedFiles())
            qnoto::FileHandler::instance().activate(fname);
    }
}

//--------------------------------------------------------------------------------------------------

QString Editors::name() const
{
    return "editors";
}

//--------------------------------------------------------------------------------------------------

QString Editors::title() const
{
    return tr("Editors");
}

//--------------------------------------------------------------------------------------------------

const Editors::MenuList& Editors::actions() const
{
    return m_actions.actions();
}

//--------------------------------------------------------------------------------------------------

qnoto::EditorInstance* Editors::currentEditor() const
{
    return m_editors[m_current].editor;
}

//--------------------------------------------------------------------------------------------------

const QMap<QString, EditorInfo>& Editors::editors() const
{
    return m_editors;
}

//--------------------------------------------------------------------------------------------------
