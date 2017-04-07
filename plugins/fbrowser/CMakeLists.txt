cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

include(GenerateExportHeader)

qnoto_init(fbrowser QT Core Widgets)

qnoto_add_plugin(${PROJECT_NAME}
    SOURCES
        src/file-browser.h
        src/file-browser.cpp
        src/list-model.h
        src/list-model.cpp
        src/list-widget.h
        src/list-widget.cpp
    LIBS
        Qt5::Core
        Qt5::Widgets
        common
)
