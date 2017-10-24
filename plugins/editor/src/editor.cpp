#include <QDebug>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QAction>
#include <QStandardPaths>
#include <QFileDialog>
#include "editor.h"
#include "syntax/manager.h"
#include "includes/ui-utils.h"
#include "parts/impl.h"
#include "parts/header.h"
#include "preferences.h"

EditorInstance::EditorInstance()
{
}

EditorInstance::~EditorInstance()
{
}

bool EditorInstance::init(const QString& fileName)
{
    m_fileName = fileName;
    QFile f(fileName);

    m_widget.reset(new EditorImpl(fileName, syntax::Manager::instance().definition(fileName)));
    m_header.reset(new EditorHeader(m_widget.data()));

    m_widget->init();
    connect(m_widget.data(), &EditorImpl::escape, this, &EditorInstance::escape);

    qnoto::setLayout<QVBoxLayout>(this,
        m_header.data(),
        m_widget.data()
    );

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    uchar* filedata = f.map(0, f.size());
    m_widget->load(reinterpret_cast<char*>(filedata));

    f.unmap(filedata);
    f.close();

    m_actions = {
        qnoto::Action<EditorImpl>(m_widget.data(), "Edit_undo").
            icon("undo").
            shortcut(QKeySequence::Undo).
            title(tr("Undo")).
            action(&EditorImpl::undo)
    };

    return true;
}

void EditorInstance::focusInEvent(QFocusEvent *)
{
    m_widget->setFocus();
}

/*void EditorInstance::populateMenu(qnoto::Editors* manager, QMenu* menu)
{
    if (!m_editMenu){
        m_editMenu.reset(new QMenu);
        QAction* undoAct = m_editMenu->addAction(
            qnoto::icon("edit-undo"),
            tr("Undo"),
            m_widget.data(), &EditorImpl::undo,
            QKeySequence::Undo
        );
        undoAct->setEnabled(m_widget->document()->isUndoAvailable());
        connect(m_widget.data(), &EditorImpl::undoAvailable, undoAct, &QAction::setEnabled);

        QAction* redoAct = m_editMenu->addAction(
            qnoto::icon("edit-redo"),
            tr("Redo"),
            m_widget.data(), &EditorImpl::redo,
            QKeySequence::Redo
        );
        redoAct->setEnabled(m_widget->document()->isRedoAvailable());
        connect(m_widget.data(), &EditorImpl::redoAvailable, redoAct, &QAction::setEnabled);

        m_editMenu->addSeparator();

        QAction* cutAct = m_editMenu->addAction(
            qnoto::icon("edit-cut"),
            tr("Cut"),
            m_widget.data(), &EditorImpl::cut,
            QKeySequence::Cut
        );
        cutAct->setEnabled(!m_widget->textCursor().selectedText().isEmpty());
        connect(m_widget.data(), &EditorImpl::copyAvailable, cutAct, &QAction::setEnabled);

        QAction* copyAct = m_editMenu->addAction(
            qnoto::icon("edit-copy"),
            tr("Copy"),
            m_widget.data(), &EditorImpl::copy,
            QKeySequence::Copy
        );
        copyAct->setEnabled(!m_widget->textCursor().selectedText().isEmpty());
        connect(m_widget.data(), &EditorImpl::copyAvailable, copyAct, &QAction::setEnabled);

        QAction* pasteAct = m_editMenu->addAction(
            qnoto::icon("edit-paste"),
            tr("Paste"),
            m_widget.data(), &EditorImpl::paste,
            QKeySequence::Paste
        );
        pasteAct->setEnabled(m_widget->canPaste());
        const QClipboard *clipboard = QApplication::clipboard();
        connect(clipboard, &QClipboard::dataChanged, [pasteAct, clipboard](){
            pasteAct->setEnabled(!clipboard->text().isEmpty());
        });

        m_editMenu->addSeparator();

        m_editMenu->addAction(
            qnoto::icon("edit-find"),
            tr("Find"),
            manager, &qnoto::Editors::showFind,
            QKeySequence::Find
        );

        m_editMenu->addAction(
            qnoto::icon("edit-find-next"),
            tr("Find next"),
            m_widget.data(), &EditorImpl::doFind,
            QKeySequence::FindNext
        );

        m_editMenu->addAction(
            qnoto::icon("edit-find-previous"),
            tr("Find previous"),
            m_widget.data(), &EditorImpl::doFind,
            QKeySequence::FindPrevious
        );
    }

    menu->clear();
    for(QAction* act: m_editMenu->actions()){
        menu->addAction(act);
    }
}*/

void EditorInstance::setSearch(const QString& text)
{
    m_widget->startFind(text);
}

QString EditorInstance::selectedText() const
{
    return m_widget->textCursor().selectedText();
}

void EditorInstance::doSearch()
{
    m_widget->doFind();
}

void EditorInstance::escape()
{
    emit qnoto::EditorInstance::escape();
    m_widget->setFocus();
}

bool EditorInstance::isModified() const
{
    return m_widget->document()->isModified() || m_fileName.isEmpty();
}

const QString& EditorInstance::fileName() const
{
    return m_fileName;
}

bool EditorInstance::save(const QString& newFileName)
{
    if (!newFileName.isEmpty())
        m_fileName = newFileName;

    QFile f(m_fileName);

    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    f.write(m_widget->text().toUtf8());
    f.close();

    m_widget->document()->setModified(false);
    return true;
}

const QList<QAction*>& EditorInstance::actions()
{
    return m_actions;
}

//--------------------------------------------------------------------------------------------------

qnoto::EditorInstance* Editor::create()
{
    return new EditorInstance;
}

QString Editor::name() const
{
    return "default-editor";
}

QString Editor::title() const
{
    return tr("Default editor");
}

qnoto::PreferencesPage* Editor::preferences() const
{
    return new Preferences;
}

