#include <QLabel>
#include <QFontMetrics>
#include "header.h"
#include "impl.h"
#include "includes/ui-utils.h"

EditorHeader::EditorHeader(EditorImpl* editor):
    m_editor(editor)
{
    setProperty("panelWidget", true);
    setMaximumHeight(22);

    QLabel* lab = new QLabel(m_editor->fileName());
    lab->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding));

    QFontMetrics met(font());

    QLabel* line = new QLabel("0");
    line->setMinimumWidth(met.width("0")*4);

    QLabel* col = new QLabel("0");
    col->setMinimumWidth(met.width("0")*4);

    auto* lay = qnoto::setLayout<QHBoxLayout>(this, lab,
        new QLabel(tr("Line:")), line,
        new QLabel(tr("Col:")), col
    );

    lay->setContentsMargins(2,2,2,2);
    lay->setSpacing(4);

    connect(m_editor, &EditorImpl::cursorPositionChanged, [this, line, col](){
        line->setText(QString("%1").arg(m_editor->textCursor().blockNumber()+1));
        col->setText(QString("%1").arg(m_editor->textCursor().columnNumber()));
    });

    connect(m_editor, &EditorImpl::modificationChanged, [this, lab](bool modified){
        lab->setText(m_editor->fileName()+(modified ? "*" : ""));
    });
}
