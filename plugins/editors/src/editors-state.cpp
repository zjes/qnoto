#include <QAction>
#include "includes/file-handler.h"
#include "editors-state.h"
#include "editors.h"

EditorsState::EditorsState(Editors* eds):
    m_eds(eds)
{
    qnoto::FileHandler& fh = qnoto::FileHandler::instance();

    connect(&fh, &qnoto::FileHandler::activated, this, &EditorsState::activated);
    connect(&fh, &qnoto::FileHandler::modified, this, &EditorsState::modified);
    connect(&fh, &qnoto::FileHandler::closed, this, &EditorsState::closed);
    connect(m_eds, &Editors::openedChanged, this, &EditorsState::openedChanged);
}

void EditorsState::modified(const QString& fileName, bool modified)
{
    if (m_eds->currentEditor() && m_eds->currentEditor()->fileName() == fileName){
        for(auto& it: m_eds->actions("File_save")){
            it->setEnabled(modified);
        }
    }

    bool saveAll = false;
    for(const auto& it: m_eds->editors()){
        if (!it.editor->isModified())
            continue;
        saveAll = true;
        break;
    }

    for(auto& it: m_eds->actions("File_saveAll")){
        it->setEnabled(saveAll);
    }
}

void EditorsState::activated(const QString& fileName)
{
    m_eds->openFile(fileName);

    for(auto& it: m_eds->actions("File_save")){
        it->setEnabled(m_eds->currentEditor()->isModified());
    }
}

void EditorsState::closed(const QString& fileName)
{
    m_eds->closeFile(fileName);
}

void EditorsState::openedChanged()
{
    for(auto& it: m_eds->actions("File_close", "File_closeAll", "File_closeAllExcept", "File_saveAs"))
        it->setEnabled(m_eds->editors().size() > 0);
}
