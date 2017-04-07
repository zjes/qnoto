cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

include(GenerateExportHeader)

qnoto_init(editors QT Core Widgets)

qnoto_add_plugin(${PROJECT_NAME}
    SOURCES
        src/editors.h
        src/editors.cpp
        src/editor-factory.h
        src/editor-factory.cpp
        src/history-navigate.h
        src/history-navigate.cpp
    LIBS
        Qt5::Core
        Qt5::Widgets
        common
)
