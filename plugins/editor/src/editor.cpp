#include <QDebug>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QAction>
#include "editor.h"
#include "syntax/manager.h"
#include "includes/ui-utils.h"
#include "editor-impl.h"
#include "editor-header.h"
#include "editor-find.h"
#include "preferences.h"

EditorInstance::~EditorInstance()
{
}

bool EditorInstance::init(const QString& fileName)
{
    QFile f(fileName);

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    uchar* filedata = f.map(0, f.size());

    m_widget.reset(new EditorImpl(fileName, syntax::Manager::instance().definition(fileName)));
    m_header.reset(new EditorHeader(m_widget.data()));
    m_find.reset(new EditorFind(m_widget.data()));

    m_widget->init();
    m_widget->load(reinterpret_cast<char*>(filedata));

    qnoto::setLayout<QVBoxLayout>(this,
        m_header.data(),
        m_widget.data(),
        m_find.data()
    );

    f.unmap(filedata);
    f.close();
    return true;
}

void EditorInstance::focusInEvent(QFocusEvent *)
{
    m_widget->setFocus();
}

void EditorInstance::populateMenu(QMenu* menu)
{
    if (!m_editMenu){
        m_editMenu.reset(new QMenu);
        QAction* undoAct = m_editMenu->addAction(
            QIcon::fromTheme("edit-undo"),
            tr("Undo"),
            m_widget.data(), &EditorImpl::undo,
            QKeySequence::Undo
        );
        undoAct->setEnabled(m_widget->document()->isUndoAvailable());
        connect(m_widget.data(), &EditorImpl::undoAvailable, undoAct, &QAction::setEnabled);

        QAction* redoAct = m_editMenu->addAction(
            QIcon::fromTheme("edit-redo"),
            tr("Redo"),
            m_widget.data(), &EditorImpl::redo,
            QKeySequence::Redo
        );
        redoAct->setEnabled(m_widget->document()->isRedoAvailable());
        connect(m_widget.data(), &EditorImpl::redoAvailable, redoAct, &QAction::setEnabled);

        m_editMenu->addSeparator();

        QAction* cutAct = m_editMenu->addAction(
            QIcon::fromTheme("edit-cut"),
            tr("Cut"),
            m_widget.data(), &EditorImpl::cut,
            QKeySequence::Cut
        );
        cutAct->setEnabled(!m_widget->textCursor().selectedText().isEmpty());
        connect(m_widget.data(), &EditorImpl::copyAvailable, cutAct, &QAction::setEnabled);

        QAction* copyAct = m_editMenu->addAction(
            QIcon::fromTheme("edit-copy"),
            tr("Copy"),
            m_widget.data(), &EditorImpl::copy,
            QKeySequence::Copy
        );
        copyAct->setEnabled(!m_widget->textCursor().selectedText().isEmpty());
        connect(m_widget.data(), &EditorImpl::copyAvailable, copyAct, &QAction::setEnabled);

        QAction* pasteAct = m_editMenu->addAction(
            QIcon::fromTheme("edit-paste"),
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
            QIcon::fromTheme("edit-find"),
            tr("Find"),
            m_find.data(), &EditorFind::activate,
            QKeySequence::Find
        );
    }

    menu->clear();
    for(QAction* act: m_editMenu->actions()){
        menu->addAction(act);
    }
}

void EditorInstance::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && m_find->isVisible()){
        m_find->hide();
        event->ignore();
        m_widget->setFocus();
    }
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

