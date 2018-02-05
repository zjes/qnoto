cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

include("common/cmake/qnoto.cmake")

project(qNoto)

add_subdirectory(common)
add_subdirectory(plugins/editor)
add_subdirectory(plugins/fbrowser)
add_subdirectory(plugins/opened-files)


add_definitions(-DQT_QUICK_DEFAULT_TEXT_RENDER_TYPE=NativeRendering)

qnoto_add_executable(qNoto
    SOURCES
        src/main.cpp
        src/qnoto.cpp
        src/qnoto.h
        src/fileicon.cpp
        src/fileicon.h
        src/fileinfo.cpp
        src/fileinfo.h
        src/utils.cpp
        src/utils.h
        src/settings.h
        src/settings.cpp
        src/plugins-model.h
        src/plugins-model.cpp
        src/selectable.h
        src/selectable.cpp

    OUTPUT ${CMAKE_SOURCE_DIR}/bin

    LIBS
        Qt5::Core
        Qt5::Qml
        Qt5::Quick
        Qt5::Gui
        Qt5::QuickControls2
        common
)

qnoto_add_resource(
    TARGET  qNoto
    SOURCES resources/qml.qrc
)

qnoto_add_resource(
    NAME    icons
    TARGET  qNoto
    SOURCES resources/icons.qrc
)
