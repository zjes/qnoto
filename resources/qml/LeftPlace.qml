import QtQuick 2.10
import QtQuick.Controls 2.3
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
