#include <QDebug>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include <QAction>
#include <QStandardPaths>
#include <QFileDialog>
#include <QResource>
#include "editor.h"
#include "syntax/manager.h"
#include "editor-instance.h"
#include "default-editor.h"

//--------------------------------------------------------------------------------------------------

Editor::Editor()
{
    QResource::registerResource("plugins/editor.rcc");

    qmlRegisterType<DefaultEditor>("DefaultEditor", 1, 0, "DefaultEditor");
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
