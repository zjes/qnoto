include(CMakeParseArguments)
include(CMakePackageConfigHelpers)
include(GenerateExportHeader)

macro(initLibs Libs)
    foreach(lib ${${Libs}})
        if (NOT TARGET ${lib})
            string(REPLACE "::" ";" split ${lib})
            list(LENGTH split splLen)
            if (${splLen} EQUAL 2)
                list(GET split 0 pref)
                list(GET split 1 mod)
                find_package(${pref} COMPONENTS ${mod})
            else()
                find_package(${lib} PATHS "${CMAKE_SOURCE_DIR}/../${lib}/build/")
            endif()
        endif()
    endforeach()
endmacro()

macro(qnoto_add_lib Name)
    cmake_parse_arguments(Args "" "OUTPUT" "SOURCES;LIBS;PUBLIC;MOC" ${ARGN})
    if(Args_MOC)
        qt5_wrap_cpp(moc ${Args_MOC})
        set(${Args_SOURCES} "${Args_SOURCES} ${moc}")
        #set_property(TARGET ${target} APPEND PROPERTY SOURCES ${moc})
    endif()

    initLibs(Args_LIBS)

    add_library(${Name} SHARED
        ${Args_PUBLIC}
        ${Args_SOURCES}
    )

    if(Args_OUTPUT)
        set_property(TARGET ${Name} PROPERTY LIBRARY_OUTPUT_DIRECTORY ${Args_OUTPUT})
    endif()


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
    cmake_parse_arguments(Args "" "OUTPUT" "SOURCES;LIBS;PUBLIC" ${ARGN})

    initLibs(Args_LIBS)

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

    if(Args_OUTPUT)
        set_property(TARGET ${Name} PROPERTY RUNTIME_OUTPUT_DIRECTORY ${Args_OUTPUT})
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

        export(TARGETS ${Name} FILE ${Name}-targets.cmake)

        install(TARGETS ${Name}
            DESTINATION lib
            #PUBLIC_HEADER DESTINATION ${INCLUDE_INSTALL_DIR}
            EXPORT ${Name}-targets
        )
        install(EXPORT ${Name}-targets DESTINATION ${CMAKE_INSTALL_DIR})
    endif()
endmacro()

macro(qnoto_add_plugin Name)
    cmake_parse_arguments(Args "" "OUTPUT" "SOURCES;LIBS;PUBLIC" ${ARGN})

    initLibs(Args_LIBS)

    add_library(${Name} SHARED
        ${Args_PUBLIC}
        ${Args_SOURCES}
    )

    set_property(TARGET ${Name} PROPERTY CXX_STANDARD 14)

    if(Args_OUTPUT)
        set_property(TARGET ${Name} PROPERTY LIBRARY_OUTPUT_DIRECTORY ${Args_OUTPUT})
    endif()

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
        $<INSTALL_INTERFACE:include/qnoto>
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

macro(qnoto_add_resource)
    cmake_parse_arguments(Args "" "NAME;TARGET;OUTPUT" "SOURCES" ${ARGN})

    set(out)
    if(TARGET ${Args_TARGET})
        get_target_property(out ${Args_TARGET} RUNTIME_OUTPUT_DIRECTORY)
        if (NOT out)
            get_target_property(out ${Args_TARGET} LIBRARY_OUTPUT_DIRECTORY)
        endif()
    endif()

    if(Args_OUTPUT)
        set(out ${Args_OUTPUT})
    endif()

    if(Args_NAME)
        set(name ${Args_NAME})
    else()
        set(name ${Args_TARGET})
    endif()

    find_package(Qt5 COMPONENTS Core)

    qt5_add_binary_resources(${name}-res
        ${Args_SOURCES}
        DESTINATION ${out}/${name}.rcc
    )

    if(TARGET ${Args_TARGET})
        add_dependencies(${name}-res ${Args_TARGET})
    endif()
endmacro()

set(CMAKE_INSTALL_DIR   share/cmake)
set(LIBS_INSTALL_DIR    local/qnoto/libs)
set(ARCHIVE_INSTALL_DIR local/qnoto/libs)
set(INCLUDE_INSTALL_DIR local/qnoto/includes)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
