import QtQuick 2.9
import QtQuick.Controls 2.2

Column {
    property alias plugin: header.selectedPlugin

    LeftHeader {
        id     : header
        width  : parent.width
        height : 25
    }

    PluginLoader {
        width  : parent.width
        height : parent.height - header.height
        plugin : header.selectedPlugin ? header.selectedPlugin.name : null
        focus  : true
    }
}
