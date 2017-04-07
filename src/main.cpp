#include <QDebug>
#include <QApplication>
#include "includes/commandline.h"
#include "includes/pluginmanager.h"

#include "includes/plugins/mainwindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    qnoto::CommandLine line("qnoto editor", {
    });

    if (!line.parse(argc, argv)){
        qWarning() << qPrintable(line.usage());
        return 1;
    }

    qnoto::PluginManager::instance().preloadPlugins();

    qnoto::MainWindowPtr main(qnoto::plugin<qnoto::MainWindow>());
    if (!main)
        return 2;

    if (line.positionalArgs().size())
        main->openFiles(line.positionalArgs());
    main->show();
    return app.exec();
}
