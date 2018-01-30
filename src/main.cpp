#include <QDebug>
#include <QApplication>
#include <QResource>
#include <QFont>

#include <QQmlContext>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QtQuickControls2/QQuickStyle>

#include "includes/commandline.h"
#include "includes/pluginmanager.h"
#include "includes/file-handler.h"
#include "qnoto.h"
#include "fileicon.h"
#include "fileinfo.h"
#include "utils.h"

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "[D] %s (%s:%u)\n", localMsg.constData(), context.file, context.line);
        break;
    case QtInfoMsg:
        fprintf(stderr, "[I] %s (%s:%u)\n", localMsg.constData(), context.file, context.line);
        break;
    case QtWarningMsg:
        fprintf(stderr, "[W] %s (%s:%u)\n", localMsg.constData(), context.file, context.line);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "[C] %s (%s:%u)\n", localMsg.constData(), context.file, context.line);
        break;
    case QtFatalMsg:
        fprintf(stderr, "[F] %s (%s:%u)\n", localMsg.constData(), context.file, context.line);
        abort();
    }
    fflush(stderr);
}

void cleanup()
{
    QResource::unregisterResource("qNoto.rcc");
    QResource::unregisterResource("icons.rcc");
}

static QObject *info_singletontype_provider(QQmlEngine* /*engine*/, QJSEngine* /*scriptEngine*/)
{
    FileInfo *info = new FileInfo();
    return info;
}

static QObject *utils_singletontype_provider(QQmlEngine* /*engine*/, QJSEngine* /*scriptEngine*/)
{
    Utils *utils = new Utils();
    return utils;
}

int main(int argc, char** argv)
{
    qInstallMessageHandler(messageOutput);
    QQuickWindow::setSceneGraphBackend(QSGRendererInterface::OpenGL);
    //QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);

    QApplication app(argc, argv);
    app.setOrganizationName("qNoto");

    qnoto::CommandLine line("qnoto editor");

    if (!line.parse(argc, argv)){
        qWarning() << qPrintable(line.usage());
        return 1;
    }

    QQuickStyle::setStyle("Fusion");

    qnoto::PluginManager::instance().preloadPlugins();

    QResource::registerResource("qNoto.rcc");
    QResource::registerResource("icons.rcc");
    QQmlApplicationEngine* engine = new QQmlApplicationEngine();

    qmlRegisterSingletonType<FileInfo>("Utils", 1, 0, "FileInfo", info_singletontype_provider);
    qmlRegisterSingletonType<FileInfo>("Utils", 1, 0, "Utils", utils_singletontype_provider);

    QNoto qnoto;
    engine->rootContext()->setContextProperty("qnoto", &qnoto);
    engine->rootContext()->setContextProperty("fileHandler", &qnoto::FileHandler::instance());

    engine->addImageProvider("fileicon", new FileIconProvider);

    engine->addImportPath("qrc:/qml/");
    engine->load("qrc:/qml/Main.qml");

    QQuickWindow *window = qobject_cast<QQuickWindow*>(engine->rootObjects().value(0));
    if (!window) {
        qWarning("Error: Root item must to be a Window class.");
        cleanup();
        exit(2);
    }

    window->show();

    int ret = app.exec();
    engine->deleteLater();
    cleanup();


//    if (line.positionalArgs().size())
//        main->openFiles(line.positionalArgs());

    return ret;
}
