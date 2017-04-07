cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

include(GenerateExportHeader)

qnoto_init(opened-files QT Core Widgets)

qnoto_add_plugin(${PROJECT_NAME}
    SOURCES
        src/opened-files.h
        src/opened-files.cpp
    LIBS
        Qt5::Core
        Qt5::Widgets
        common
)
