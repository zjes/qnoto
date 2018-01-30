import QtQuick 2.10
import QtQuick.Controls 2.3
import Controls 1.0

Frame {
    padding: 1

    property var selectedPlugin: null

    Item {
        anchors.fill: parent
        ComboBox {
            id: control
            model: qnoto.pluginsModel("qNoto.ToolPlugin")
            height: parent.height
            flat: true
            anchors.left: parent.left
            anchors.right: _close.left
            textRole: "caption"
            currentIndex: selectedPlugin ? indexByName(selectedPlugin.name) : 0
            onCurrentIndexChanged: {
                selectedPlugin = model.value(currentIndex);
            }
        }
        ToolButton {
            id: _close
            text: qsTr("Close")
            height: parent.height
            width: parent.height
            anchors.right: _addSplit.left
            flat: true
            icon.source: "qrc:/icons/close.png"
            display: AbstractButton.IconOnly
        }
        ToolButton {
            id: _addSplit
            text: qsTr("Add split")
            width: parent.height
            height: parent.height
            anchors.right: parent.right
            flat: true
            icon.source: "qrc:/icons/split-horizontal.png"
            display: AbstractButton.IconOnly
            onClicked: {
            }
        }
    }

    function indexByName(name)
    {
        for(var i = 0; i < control.model.length(); ++i){
            var plug = control.model.value(i);
            if (plug.name === name)
                return i;
        }
        return 0;
    }

    onSelectedPluginChanged: {
        if (!selectedPlugin)
            return;

        control.currentIndex = indexByName(selectedPlugin.name);
    }
}
