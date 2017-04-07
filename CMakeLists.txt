cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_AUTOUIC ON)

add_subdirectory(common)
add_subdirectory(plugins/mainwindow)
add_subdirectory(plugins/editor)
add_subdirectory(plugins/fbrowser)
add_subdirectory(plugins/editors)
add_subdirectory(plugins/opened-files)


qnoto_init(qNoto QT Core Widgets)

qnoto_add_executable(${PROJECT_NAME}
    SOURCES
        src/main.cpp
    LIBS
        Qt5::Core
        Qt5::Widgets
        common
)
