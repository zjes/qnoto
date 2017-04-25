#include <QDebug>
#include <QTextCursor>
#include <QKeyEvent>
#include "includes/ui-utils.h"
#include "editors-find.h"
#include "ui_editors-find.h"

EditorsFind::EditorsFind():
    m_ui(new Ui::EditorFind)
{
    m_ui->setupUi(this);
    setVisible(false);
    setProperty("panelWidget2", true);

    m_ui->closeBtn->setIcon(qnoto::icon("window-close"));
    m_ui->findNextBtn->setIcon(qnoto::icon("go-next"));
    m_ui->findPrevBtn->setIcon(qnoto::icon("go-previous"));

    connect(m_ui->closeBtn, &QToolButton::clicked, this, &EditorsFind::hide);
    connect(m_ui->findEdt,  &QLineEdit::textChanged, this, &EditorsFind::textChanged);

    m_searchTimer.setSingleShot(true);
    connect(&m_searchTimer, &QTimer::timeout, this, &EditorsFind::doSearch);
}

EditorsFind::~EditorsFind()
{
}

void EditorsFind::setEditor(qnoto::EditorInstance* editor)
{
    m_editor = editor;
    if (isVisible() && !m_ui->findEdt->text().isEmpty()){
        doSearch();
    }
}

void EditorsFind::textChanged(const QString&)
{
    m_searchTimer.start(100);
}

void EditorsFind::doSearch()
{
    m_editor->setSearch(m_ui->findEdt->text());
}

void EditorsFind::showEvent(QShowEvent*)
{
    m_ui->findEdt->setFocus();
    QString selText = m_editor->selectedText();
    if (!selText.isEmpty() && !selText.contains("\n")){
        m_ui->findEdt->setText(selText);
    }
    m_ui->replaceAllBtn->setEnabled(!m_ui->replaceEdt->text().isEmpty());
    m_ui->replaceBtn->setEnabled(!m_ui->replaceEdt->text().isEmpty());
    m_ui->replaceFindBtn->setEnabled(!m_ui->replaceEdt->text().isEmpty());
    m_ui->findEdt->selectAll();
    if (!m_ui->findEdt->text().isEmpty())
        doSearch();
}

void EditorsFind::hideEvent(QHideEvent*)
{
    m_editor->setSearch();
    m_editor->setFocus();
}

void EditorsFind::activate()
{
    if (isVisible())
        showEvent(nullptr);
    else
        setVisible(true);
}

void EditorsFind::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        m_editor->doSearch();
    } else if (event->key() == Qt::Key_Escape){
        m_editor->setFocus();
    } else {
        QWidget::keyPressEvent(event);
    }
}
