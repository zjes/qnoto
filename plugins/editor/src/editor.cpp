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
    connect(m_widget.data(), &EditorImpl::showFind, this, &EditorInstance::showFind);

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

    auto addAction = [this](const QString& name){
        return qnoto::Action<EditorImpl>(m_widget.data(), name);
    };

    auto separator = [this](const QString& name){
        QAction* act = new QAction(this);
        act->setSeparator(true);
        act->setProperty("name", name+"_");
        return act;
    };

    m_actions = {
        addAction("Edit_undo").
            icon("edit-undo").
            shortcut(QKeySequence::Undo).
            title(tr("Undo")).
            action(&EditorImpl::undo).
            enabled(m_widget->document()->isUndoAvailable()),
        addAction("Edit_redo").
            icon("edit-redo").
            shortcut(QKeySequence::Redo).
            title(tr("Redo")).
            action(&EditorImpl::redo).
            enabled(m_widget->document()->isRedoAvailable()),

        separator("Edit_"),

        addAction("Edit_cut").
            icon("edit-cut").
            shortcut(QKeySequence::Cut).
            title(tr("Cut")).
            action(&EditorImpl::cut).
            enabled(!m_widget->textCursor().selectedText().isEmpty()),
        addAction("Edit_copy").
            icon("edit-copy").
            shortcut(QKeySequence::Copy).
            title(tr("Copy")).
            action(&EditorImpl::copy).
            enabled(!m_widget->textCursor().selectedText().isEmpty()),
        addAction("Edit_paste").
            icon("edit-paste").
            shortcut(QKeySequence::Paste).
            title(tr("Paste")).
            action(&EditorImpl::paste).
            enabled(m_widget->canPaste()),

        separator("Edit_"),

        addAction("Edit_find").
            icon("edit-find").
            shortcut(QKeySequence::Find).
            title(tr("Find && replace")).
            action(&EditorImpl::showFind),

        addAction("Edit_findNext").
            icon("edit-findnext").
            shortcut(QKeySequence::FindNext).
            title(tr("Find next")).
            action(&EditorImpl::doFind).
            enabled(false),

        addAction("Edit_findPrev").
            icon("edit-findprev").
            shortcut(QKeySequence::FindPrevious).
            title(tr("Find previous")).
            action(&EditorImpl::doFind).
            enabled(false),
    };

    connect(m_widget->document(), &QTextDocument::undoAvailable, action("Edit_undo"), &QAction::setEnabled);
    connect(m_widget->document(), &QTextDocument::redoAvailable, action("Edit_redo"), &QAction::setEnabled);
    connect(m_widget.data(), &EditorImpl::copyAvailable, action("Edit_copy"), &QAction::setEnabled);
    connect(m_widget.data(), &EditorImpl::copyAvailable, action("Edit_cut"), &QAction::setEnabled);
    connect(m_widget.data(), &EditorImpl::findAvail, action("Edit_findNext"), &QAction::setEnabled);
    connect(m_widget.data(), &EditorImpl::findAvail, action("Edit_findPrev"), &QAction::setEnabled);

    const QClipboard *clipboard = QApplication::clipboard();
    connect(clipboard, &QClipboard::dataChanged, [this, clipboard](){
        action("Edit_paste")->setEnabled(!clipboard->text().isEmpty());
    });
    return true;
}

void EditorInstance::focusInEvent(QFocusEvent *)
{
    m_widget->setFocus();
}

QAction* EditorInstance::action(const QString& name) const
{
    for(const auto& it: m_actions){
        if (it->property("name").toString() == name)
            return it;
    }
    return nullptr;
}

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

