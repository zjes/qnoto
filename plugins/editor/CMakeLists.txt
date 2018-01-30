cmake_minimum_required(VERSION 3.5)
cmake_policy(VERSION 3.5)

qnoto_add_plugin(editor
    SOURCES
        src/editor.h
        src/editor.cpp
        src/editor-instance.h
        src/editor-instance.cpp
        src/default-editor.h
        src/default-editor.cpp
        src/settings.h
        src/settings.cpp

        src/syntax/common.h
        src/syntax/manager.h
        src/syntax/manager.cpp
        src/syntax/parser.h
        src/syntax/parser.cpp
        src/syntax/definition.h
        src/syntax/definition.cpp
        src/syntax/keywordlist.h
        src/syntax/keywordlist.cpp
        src/syntax/context.h
        src/syntax/context.cpp
        src/syntax/highlighting.h
        src/syntax/highlighting.cpp
        src/syntax/item-data.h
        src/syntax/item-data.cpp
        src/syntax/theme.h
        src/syntax/theme.cpp
        src/syntax/syntaxhighlighter.cpp
        src/syntax/syntaxhighlighter.h
        src/syntax/formater.cpp
        src/syntax/formater.h

        src/syntax/rules/rule.h
        src/syntax/rules/rule.cpp
        src/syntax/rules/anychar.h
        src/syntax/rules/anychar.cpp
        src/syntax/rules/detect2char.h
        src/syntax/rules/detect2char.cpp
        src/syntax/rules/detectchar.h
        src/syntax/rules/detectchar.cpp
        src/syntax/rules/detectidentifier.h
        src/syntax/rules/detectidentifier.cpp
        src/syntax/rules/detectspaces.cpp
        src/syntax/rules/detectspaces.h
        src/syntax/rules/float.cpp
        src/syntax/rules/float.h
        src/syntax/rules/hlcchar.cpp
        src/syntax/rules/hlcchar.h
        src/syntax/rules/int.cpp
        src/syntax/rules/int.h
        src/syntax/rules/hlchex.h
        src/syntax/rules/hlchex.cpp
        src/syntax/rules/hlcoct.h
        src/syntax/rules/hlcoct.cpp
        src/syntax/rules/hlcstringchar.h
        src/syntax/rules/hlcstringchar.cpp
        src/syntax/rules/keyword.h
        src/syntax/rules/keyword.cpp
        src/syntax/rules/linecontinue.h
        src/syntax/rules/linecontinue.cpp
        src/syntax/rules/rangedetect.h
        src/syntax/rules/rangedetect.cpp
        src/syntax/rules/regexpr.h
        src/syntax/rules/regexpr.cpp
        src/syntax/rules/stringdetect.h
        src/syntax/rules/stringdetect.cpp
        src/syntax/rules/worddetect.h
        src/syntax/rules/worddetect.cpp
        src/syntax/rules/includerules.h
        src/syntax/rules/includerules.cpp

    OUTPUT ${CMAKE_SOURCE_DIR}/bin/plugins

    LIBS
        Qt5::Core
        Qt5::Xml
        Qt5::Concurrent
        Qt5::Quick
        common
)

qnoto_add_resource(
    TARGET  editor
    SOURCES resources/qml.qrc
)
