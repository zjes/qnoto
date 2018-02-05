import QtQuick 2.9
import QtQuick.Controls 2.2
import Controls 1.0

SplitView {
    orientation: SplitView.vertical

    ToolPlugin {
        width: parent.width
        height: parent.height/2
        plugin: qnoto.plugin("qNoto.ToolPlugin.FileBrowser")
    }

    ToolPlugin {
        width: parent.width
        height: parent.height/2
        plugin: qnoto.plugin("qNoto.ToolPlugin.OpenedFiles")
    }
}
