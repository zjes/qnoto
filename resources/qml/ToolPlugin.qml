import QtQuick 2.10
import QtQuick.Controls 2.3

Column {
    property alias plugin: header.selectedPlugin

    LeftHeader {
        id     : header
        width  : parent.width
        height : 30
    }

    PluginLoader {
        width  : parent.width
        height : parent.height - header.height
        plugin : header.selectedPlugin ? header.selectedPlugin.name : null
        focus  : true
    }
}
