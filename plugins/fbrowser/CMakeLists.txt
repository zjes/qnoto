cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

qnoto_add_plugin(fbrowser
    SOURCES
        src/file-browser.h
        src/file-browser.cpp
        src/file-handler.h
        src/file-handler.cpp
        src/file-entriesmodel.h
        src/file-entriesmodel.cpp
        src/file-pathmodel.h
        src/file-pathmodel.cpp
        src/settings.h
        src/settings.cpp

    OUTPUT ${CMAKE_SOURCE_DIR}/bin/plugins

    LIBS
        Qt5::Core
        Qt5::Qml
        common
)

qnoto_add_resource(
    TARGET  fbrowser
    SOURCES resources/qml.qrc
)
