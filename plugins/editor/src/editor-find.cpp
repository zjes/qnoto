#include <QDebug>
#include <QTextCursor>
#include "editor-find.h"
#include "ui_editor-find.h"
#include "editor-impl.h"

EditorFind::EditorFind(EditorImpl *parent):
    m_editor(parent),
    m_ui(new Ui::EditorFind)
{
    m_ui->setupUi(this);
    setVisible(false);

    m_ui->closeBtn->setIcon(QIcon::fromTheme("window-close"));
    connect(m_ui->closeBtn, &QToolButton::clicked, this, &EditorFind::hide);
    connect(m_ui->findEdt,  &QLineEdit::textChanged, m_editor, &EditorImpl::startFind);
}

EditorFind::~EditorFind()
{
}

void EditorFind::showEvent(QShowEvent*)
{
    m_ui->findEdt->setFocus();
    QString selText = m_editor->textCursor().selectedText();
    if (!selText.isEmpty() && !selText.contains("\n")){
        m_ui->findEdt->setText(selText);
        m_ui->findEdt->selectAll();
    }
    m_ui->replaceAllBtn->setEnabled(!m_ui->replaceEdt->text().isEmpty());
    m_ui->replaceBtn->setEnabled(!m_ui->replaceEdt->text().isEmpty());
    m_ui->replaceFindBtn->setEnabled(!m_ui->replaceEdt->text().isEmpty());
}

void EditorFind::hideEvent(QHideEvent*)
{
    m_editor->setFocus();
    m_editor->unmark();
}

void EditorFind::activate()
{
    if (isVisible())
        showEvent(nullptr);
    else
        setVisible(true);
}
