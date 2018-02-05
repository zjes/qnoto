#include <QQuickTextDocument>
#include <QTimer>
#include <QQmlEngine>
#include "default-editor.h"
#include "syntax/manager.h"
#include "includes/file-handler.h"
#include "settings.h"

DefaultEditor::DefaultEditor()
{
    syntax::ThemePtr theme = syntax::ThemePtr::create();
    QQmlEngine::setObjectOwnership(theme.data(), QQmlEngine::CppOwnership);

    if (!theme->load("themes/breeze-dark.theme"))
        return;

    m_theme = theme;
}

const QString& DefaultEditor::fileName() const
{
    return m_fileName;
}

void DefaultEditor::setFileName(const QString& fname)
{
    QFile f(fname);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    auto& sets = Settings::instance();

    m_fileName = fname;
    m_definition = syntax::Manager::instance().definition(fname);
    m_syntax.reset(new syntax::Highlighting(m_doc, m_definition, sets.showWhite));
    m_syntax->setTheme(m_theme);

    //m_doc->setWordWrapMode(sets.wordWrap ? QTextOption::WordWrap : QTextOption::NoWrap);

    QTextOption opt = m_doc->defaultTextOption();
    if (sets.showWhite)
        opt.setFlags(opt.flags() | QTextOption::ShowTabsAndSpaces);
    if (sets.showEndl)
        opt.setFlags(opt.flags() | QTextOption::ShowLineAndParagraphSeparators);
    m_doc->setDefaultTextOption(opt);

    //setTabStopWidth(4 * m_doc->fontMetrics().width(' '));

    uchar* filedata = f.map(0, f.size());
    m_text = reinterpret_cast<char*>(filedata);
    emit textChanged();

    f.unmap(filedata);
    f.close();
}

QQuickItem* DefaultEditor::target()
{
    return m_target;
}

void DefaultEditor::setTarget(QQuickItem *target)
{
    m_target = target;
    qDebug() << m_target;
    QVariant doc = m_target->property("textDocument");
    if (doc.canConvert<QQuickTextDocument*>()) {
        QQuickTextDocument *qqdoc = doc.value<QQuickTextDocument*>();
        if (qqdoc)
            m_doc = qqdoc->textDocument();
    }

    emit targetChanged();
    connect(m_doc, &QTextDocument::modificationChanged, this, &DefaultEditor::modificationChanged);
}

void DefaultEditor::modificationChanged(bool modification)
{
    qnoto::FileHandler::instance().fileChange(m_fileName, modification);
    emit isModifiedChanged();
}

const QString& DefaultEditor::text() const
{
    return m_text;
}

void DefaultEditor::setText(const QString& text)
{
    m_text = text;
    emit textChanged();
}

syntax::Theme* DefaultEditor::theme() const
{
    return m_theme.data();
}

bool DefaultEditor::isModified() const
{
    return m_doc ? m_doc->isModified() : false;
}
