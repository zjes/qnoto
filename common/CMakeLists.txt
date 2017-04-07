cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

include("cmake/qnoto.cmake")

qnoto_init(common QT Core Widgets)

qnoto_add_lib(${PROJECT_NAME}
    PUBLIC
        includes/commandline.h
        includes/convert.h
        includes/plugin.h
        includes/pluginmanager.h
        includes/file-handler.h
        includes/color-select.h
        includes/ui-utils.h

        includes/plugins/mainwindow.h
        includes/plugins/editor.h
        includes/plugins/leftside.h
        includes/plugins/editors.h
    SOURCES
        src/plugin.cpp
        src/pluginmanager.cpp
        src/plugins-list.cpp
        src/file-handler.cpp
        src/color-select.cpp
    LIBS
        Qt5::Core
        Qt5::Widgets
)
