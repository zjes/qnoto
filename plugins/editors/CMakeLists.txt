cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

include(GenerateExportHeader)

qnoto_init(editors QT Core Widgets)

qnoto_add_plugin(${PROJECT_NAME}
    SOURCES
        src/editors.h
        src/editors.cpp
        src/editors-factory.h
        src/editors-factory.cpp
        src/editors-find.h
        src/editors-find.cpp
        src/editors-find.ui
        src/history-navigate.h
        src/history-navigate.cpp
        src/editors-state.h
        src/editors-state.cpp
        src/editors-actions.h
        src/editors-actions.cpp
        src/settings.h
        src/settings.cpp
    LIBS
        Qt5::Core
        Qt5::Widgets
        common
)
