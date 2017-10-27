#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <QMenu>
#include "editors-actions.h"
#include "includes/ui-utils.h"
#include "includes/file-handler.h"

#include "editors.h"
#include "settings.h"

EditorsActions::EditorsActions(Editors* eds):
    m_eds(eds),
    m_switcher(eds)
{
    auto action = [this](const QString& name){
        return qnoto::Action<EditorsActions>(this, name);
    };

    auto separator = [this](const QString& name){
        QAction* act = new QAction(this);
        act->setSeparator(true);
        act->setProperty("name", name+"_");
        return act;
    };

    m_actions = {
        action("File_new").
            title(tr("File New")).
            icon("file-new").
            shortcut(QKeySequence::New).
            action(&EditorsActions::newFileSlot),

        action("File_open").
            title(tr("File Open...")).
            icon("file-open").
            shortcut(QKeySequence::Open).
            action(&EditorsActions::openFileSlot),

        action("File_recent").
            title(tr("Recent Files")),

        separator("File"),

        action("File_close").title(tr("Close '%1'")).
            icon("file-close").
            shortcut(QKeySequence::Close).
            action(&EditorsActions::closeFileSlot).
            enabled(false),

        action("File_closeAllExcept").
            title(tr("Close All except '%1'")).
            icon("file-close").
            action(&EditorsActions::closeAllExceptSlot).
            enabled(false),

        action("File_closeAll").
            title(tr("Close All")).
            icon("file-close").
            action(&EditorsActions::closeAllSlot).
            enabled(false),

        separator("File"),

        action("File_save").
            title(tr("Save '%1'")).
            icon("file-save").
            shortcut(QKeySequence::Save).
            action(&EditorsActions::saveFileSlot).
            enabled(false),

        action("File_saveAs").
            title("Save '%1' As...").
            icon("file-saveas").
            shortcut(QKeySequence::SaveAs).
            action(&EditorsActions::saveAsFileSlot).
            enabled(false),

        action("File_saveAll").
            title(tr("Save All")).
            icon("file-saveall").
            action(&EditorsActions::saveAllSlot).
            enabled(false),

        action("Window_prev").
            title(tr("Previous open document in history")).
            icon("window-prev").
            //action(&EditorsActions::prevWindowSlot).
            shortcut(Qt::ControlModifier + Qt::Key_Tab).
            enabled(false),

        action("Window_next").
            title(tr("Next open document in history")).
            icon("window-next").
            //action(&EditorsActions::nextWindowSlot).
            shortcut(Qt::ControlModifier + Qt::ShiftModifier + Qt::Key_Tab).
            enabled(false),
    };

    m_switcher.setForwardAction(actions("Window_prev")[0]);
    m_switcher.setBackwardAction(actions("Window_next")[0]);

    QMenu* recent = new QMenu(m_eds);
    QAction* act = actions("File_recent")[0];
    act->setMenu(recent);

    refreshRecent();
}

//--------------------------------------------------------------------------------------------------

const qnoto::Editors::MenuList& EditorsActions::actions() const
{
    return m_actions;
}

//--------------------------------------------------------------------------------------------------

QAction* EditorsActions::findAction(const QString& actName) const
{
    for(const auto& it: m_actions){
        if (it->property("name").toString() == actName)
            return it;
    }
    return nullptr;
}

//--------------------------------------------------------------------------------------------------

void EditorsActions::newFileSlot()
{
    qnoto::FileHandler::instance().activate("");
}

//--------------------------------------------------------------------------------------------------

void EditorsActions::openFileSlot()
{
    QString fileName = QFileDialog::getOpenFileName(m_eds, tr("Open file"), lastDir(), tr("All Files (*.*)"));
    if (!fileName.isEmpty()){
        Settings::setLastDir(QFileInfo(fileName).path());
        qnoto::FileHandler::instance().activate(fileName);
    }
}

//--------------------------------------------------------------------------------------------------

int EditorsActions::savePrompt(const QString& message) const
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setInformativeText(tr("Do you want to save your changes?"));
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec();
}

//--------------------------------------------------------------------------------------------------

QString EditorsActions::lastDir() const
{
    QString last = Settings::lastDir();
    if (last.isEmpty()){
        auto paths = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
        if (paths.size())
            return paths.at(0);
    }
    return last;
}

//--------------------------------------------------------------------------------------------------

void EditorsActions::closeFileSlot()
{
    if (m_eds->currentEditor()->isModified()){
        int ret = savePrompt(tr("The document has been modified."));

        if (ret == QMessageBox::Cancel)
            return;

        if (ret == QMessageBox::Save)
            m_eds->currentEditor()->save();
    }
    qnoto::FileHandler::instance().close(m_eds->currentEditor()->fileName());
}

//--------------------------------------------------------------------------------------------------

void EditorsActions::closeAllExceptSlot()
{
    bool mod = false;
    for(const auto& it: m_eds->editors()){
        if (it.editor == m_eds->currentEditor() || !it.editor->isModified())
            continue;
        mod = true;
        break;
    }

    if (mod){
        int ret = savePrompt(tr("Some documents has been modified."));

        if (ret == QMessageBox::Cancel)
            return;

        if (ret == QMessageBox::Save){
            for(const auto& it: m_eds->editors()){
                if (it.editor != m_eds->currentEditor() && it.editor->isModified())
                    it.editor->save();
            }
        }
    }

    for(const auto& it: m_eds->editors().keys()){
        if (it != m_eds->currentEditor()->fileName())
            qnoto::FileHandler::instance().close(it);
    }
}

//--------------------------------------------------------------------------------------------------

void EditorsActions::closeAllSlot()
{
    bool mod = false;
    for(const auto& it: m_eds->editors()){
        if (!it.editor->isModified())
            continue;
        mod = true;
        break;
    }

    if (mod){
        int ret = savePrompt(tr("Some documents has been modified."));

        if (ret == QMessageBox::Cancel)
            return;

        if (ret == QMessageBox::Save)
            saveAllSlot();
    }

    for(const auto& it: m_eds->editors().keys()){
        qnoto::FileHandler::instance().close(it);
    }
}

//--------------------------------------------------------------------------------------------------

void EditorsActions::saveFileSlot()
{
    m_eds->currentEditor()->save();
}

//--------------------------------------------------------------------------------------------------

void EditorsActions::saveAsFileSlot()
{
}

//--------------------------------------------------------------------------------------------------

void EditorsActions::saveAllSlot()
{
    for(const auto& it: m_eds->editors()){
        if (it.editor->isModified())
            it.editor->save();
    }
}

//--------------------------------------------------------------------------------------------------

bool EditorsActions::canClose() const
{
    bool mod = false;
    for(const auto& it: m_eds->editors()){
        if (!it.editor->isModified())
            continue;
        mod = true;
        break;
    }

    if (mod){
        int ret = savePrompt(tr("Some documents has been modified."));
        if (ret == QMessageBox::Cancel)
            return false;

        if (ret == QMessageBox::Save){
            for(const auto& it: m_eds->editors()){
                if (it.editor->isModified())
                    it.editor->save();
            }
        }
    }
    return true;
}

//--------------------------------------------------------------------------------------------------

void EditorsActions::refreshRecent()
{
    QAction* act = actions("File_recent")[0];
    QMenu* recent = act->menu();
    recent->clear();
    for(const QString& file: Settings::recentFiles()){
        recent->addAction(
            qnoto::Action<QAction>(act, "File_recent").
                title(QFileInfo(file).fileName()).
                action([this, file](){
                    qnoto::FileHandler::instance().activate(file);
                })
        );
    }
}

