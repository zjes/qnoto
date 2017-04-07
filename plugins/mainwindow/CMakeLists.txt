cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

include(GenerateExportHeader)

qnoto_init(mainwindow QT Core Widgets)

set(CMAKE_AUTORCC ON)

qt5_add_binary_resources(${PROJECT_NAME}-res
    src/resources.qrc
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources.rcc
)

qnoto_add_plugin(${PROJECT_NAME}
    SOURCES
        src/mainwindow.h
        src/mainwindow.cpp
        src/window-impl.ui
        src/window-impl.h
        src/window-impl.cpp
        src/left-panel.h
        src/left-panel.cpp
        src/left-side-containter.h
        src/left-side-containter.cpp
        src/qnoto-style.h
        src/qnoto-style.cpp
        src/resources.qrc
        src/preferences.h
        src/preferences.cpp
        src/preferences.ui
    LIBS
        Qt5::Core
        Qt5::Widgets
        common
)
