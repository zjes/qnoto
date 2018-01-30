cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

qnoto_add_lib(common
    PUBLIC
        includes/commandline.h
        includes/convert.h
        includes/plugin.h
        includes/pluginmanager.h
        includes/file-handler.h
        includes/color-select.h
        includes/ui-utils.h
        includes/settings.h
    SOURCES
        src/plugin.cpp
        src/pluginmanager.cpp
        src/plugins-list.cpp
        src/file-handler.cpp
        src/color-select.cpp
        src/settings.cpp

    OUTPUT ${CMAKE_SOURCE_DIR}/bin

    LIBS
        Qt5::Core
        Qt5::Widgets
        Qt5::Qml
        Qt5::Quick
)
