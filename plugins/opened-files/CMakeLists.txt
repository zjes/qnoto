cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

qnoto_add_resource(
    NAME    opened-files
    SOURCES resources/qml.qrc
    OUTPUT  ${CMAKE_SOURCE_DIR}/bin/plugins
)
