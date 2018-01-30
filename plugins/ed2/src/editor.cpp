#include <QDebug>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QAction>
#include <QStandardPaths>
#include <QFileDialog>
#include <QResource>
#include "editor.h"
#include "includes/ui-utils.h"
#include "editor-instance.h"
#include "codeeditor.h"

#include "syntax/theme.h"
//#include "syntax/manager.h"

//--------------------------------------------------------------------------------------------------

Editor::Editor()
{
    QResource::registerResource("plugins/ed2-res.rcc");

    qmlRegisterType<CodeEditor>("CodeEditor", 1, 0, "CodeEditor");
    qmlRegisterType<syntax::Theme>("Theme", 1, 0, "Theme");
}

QString Editor::name() const
{
    return "default-editor";
}

QString Editor::title() const
{
    return tr("Default editor");
}

QObject* Editor::create(QObject* parent)
{
    auto* comp = createComponent(parent, "qrc:/qml/Editor.qml");
    comp->setContext("editor", new EditorInstance);
    return comp;
}
