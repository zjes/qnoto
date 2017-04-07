include(CMakeParseArguments)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

macro(qnoto_init Name)
    project(${Name})

    cmake_parse_arguments(Args "" "" "QT;MODULES" ${ARGN})

    if(NOT MSVC)
        add_definitions(-fPIC -Wall -Wextra -pedantic -Wformat=2 -Wnon-virtual-dtor -std=c++14 -Werror)
        set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed -z relro ${CMAKE_SHARED_LINKER_FLAGS}")

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            add_definitions(-g3 -O0)
        else()
            add_definitions(-O3)
        endif()
    else()
        if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            set(CMAKE_CXX_FLAGS "/W4 /WX /GS /Gm- /MP /sdl /wd\"4714\" /wd\"4127\" /EHsc /MDd /RTC1")
        else()
            set(CMAKE_CXX_FLAGS "/W4 /WX /GS /Gm- /MP /sdl /wd\"4714\" /wd\"4127\" /EHsc /MD")
        endif()
    endif()

    if(Args_QT)
        find_package(Qt5 COMPONENTS ${Args_QT})
        include_directories(Qt5)
    endif()

    if(Args_MODULES)
        foreach(dep ${Args_MODULES})
            message("${CMAKE_SOURCE_DIR}/../${dep}/build/")
            find_package(${dep} PATHS "${CMAKE_SOURCE_DIR}/../${dep}/build/")
        endforeach()
    endif()

    set(CMAKE_INSTALL_DIR   share/cmake)
    set(LIBS_INSTALL_DIR    local/qnoto/libs)
    set(ARCHIVE_INSTALL_DIR local/qnoto/libs)
    set(INCLUDE_INSTALL_DIR local/qnoto/includes)

    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOMOC ON)
endmacro()

macro(qnoto_add_lib Name)
    cmake_parse_arguments(Args "" "" "SOURCES;LIBS;PUBLIC;MOC" ${ARGN})
    if(Args_MOC)
        qt5_wrap_cpp(moc ${Args_MOC})
        set(${Args_SOURCES} "${Args_SOURCES} ${moc}")
        #set_property(TARGET ${target} APPEND PROPERTY SOURCES ${moc})
    endif()

    add_library(${Name} SHARED
        ${Args_PUBLIC}
        ${Args_SOURCES}
    )

    set_property(TARGET ${Name} PROPERTY CXX_STANDARD 14)
    target_link_libraries(${Name}
        ${Args_LIBS}
    )

    string(TOUPPER ${Name} cname)
    string(TOLOWER ${Name} lname)

    generate_export_header(${Name}
        BASE_NAME ${cname}
        EXPORT_MACRO_NAME ${cname}_EXPORT
        EXPORT_FILE_NAME ${lname}-export.h
    )

    target_include_directories(${Name} SYSTEM PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include/ori>
    )

    set(CONFFILE ${CMAKE_CURRENT_SOURCE_DIR}/${lname}-config.cmake.in)
    if(EXISTS ${CONFFILE})
        configure_package_config_file(
            ${CONFFILE}
            ${CMAKE_CURRENT_BINARY_DIR}/${lname}-config.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_DIR}
            PATH_VARS INCLUDE_INSTALL_DIR LIBS_INSTALL_DIR ARCHIVE_INSTALL_DIR CMAKE_INSTALL_DIR
        )
    endif()

    export(TARGETS ${Name} FILE ${lname}-targets.cmake)

    install(TARGETS ${Name}
        DESTINATION lib
        #PUBLIC_HEADER DESTINATION ${INCLUDE_INSTALL_DIR}
        EXPORT ${lname}-targets
    )
    install(EXPORT ${lname}-targets DESTINATION ${CMAKE_INSTALL_DIR})
endmacro()

macro(qnoto_add_executable Name)
    cmake_parse_arguments(Args "" "" "SOURCES;LIBS;PUBLIC" ${ARGN})

    if (WIN32)
        add_executable(${Name}
            ${Args_PUBLIC}
            ${Args_SOURCES}
        )
        target_link_libraries(${Name}
            Qt5::WinMain
        )
    else()
        add_executable(${Name}
            ${Args_PUBLIC}
            ${Args_SOURCES}
        )
    endif()

    set_property(TARGET ${Name} PROPERTY CXX_STANDARD 14)
    target_link_libraries(${Name}
        ${Args_LIBS}
    )

    target_include_directories(${Name} SYSTEM PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include/ori>
    )

    set(CONFFILE ${CMAKE_CURRENT_SOURCE_DIR}/${Name}-config.cmake.in)
    if(EXISTS ${CONFFILE})
        configure_package_config_file(
            ${CONFFILE}
            ${CMAKE_CURRENT_BINARY_DIR}/${Name}-config.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_DIR}
            PATH_VARS INCLUDE_INSTALL_DIR LIBS_INSTALL_DIR ARCHIVE_INSTALL_DIR CMAKE_INSTALL_DIR
        )
    endif()

    export(TARGETS ${Name} FILE ${Name}-targets.cmake)

    install(TARGETS ${Name}
        DESTINATION lib
        #PUBLIC_HEADER DESTINATION ${INCLUDE_INSTALL_DIR}
        EXPORT ${Name}-targets
    )
    install(EXPORT ${Name}-targets DESTINATION ${CMAKE_INSTALL_DIR})
endmacro()

macro(qnoto_add_plugin Name)
    cmake_parse_arguments(Args "" "" "SOURCES;LIBS;PUBLIC" ${ARGN})

    add_library(${Name} SHARED
        ${Args_PUBLIC}
        ${Args_SOURCES}
    )

    set_property(TARGET ${Name} PROPERTY CXX_STANDARD 14)
    set_property(TARGET ${Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins)
    set_property(TARGET ${Name} PROPERTY LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}/plugins)
    target_link_libraries(${Name}
        ${Args_LIBS}
    )


    string(TOUPPER ${Name} cname)
    string(TOLOWER ${Name} lname)

    generate_export_header(${Name}
        BASE_NAME ${cname}
        EXPORT_MACRO_NAME ${cname}_EXPORT
        EXPORT_FILE_NAME ${lname}-export.h
    )

    target_include_directories(${Name} SYSTEM PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include/ori>
    )

    set(CONFFILE ${CMAKE_CURRENT_SOURCE_DIR}/${lname}-config.cmake.in)
    if(EXISTS ${CONFFILE})
        configure_package_config_file(
            ${CONFFILE}
            ${CMAKE_CURRENT_BINARY_DIR}/${lname}-config.cmake
            INSTALL_DESTINATION ${CMAKE_INSTALL_DIR}
            PATH_VARS INCLUDE_INSTALL_DIR LIBS_INSTALL_DIR ARCHIVE_INSTALL_DIR CMAKE_INSTALL_DIR
        )
    endif()

    export(TARGETS ${Name} FILE ${lname}-targets.cmake)

    install(TARGETS ${Name}
        DESTINATION lib
        #PUBLIC_HEADER DESTINATION ${INCLUDE_INSTALL_DIR}
        EXPORT ${lname}-targets
    )
    install(EXPORT ${lname}-targets DESTINATION ${CMAKE_INSTALL_DIR})
endmacro()
