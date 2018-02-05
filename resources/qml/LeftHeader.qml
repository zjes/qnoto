import QtQuick 2.9
import QtQuick.Controls 2.2
import Controls 1.0

Frame {
    padding: 1
    id: header

    property var selectedPlugin: null
    property var model

    Item {
        anchors.fill: parent
        /*ComboBox {
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
            indicator: Image {
                source: ""
            }
        }*/
        Label {
            id: title
            height: parent.height
            verticalAlignment: Label.AlignVCenter
            horizontalAlignment: Label.AlignLeft
        }
        ToolBtn {
            id: _close
            text: qsTr("Close")
            height: parent.height
            width: parent.height
            anchors.right: _addSplit.left
            flat: true
            icon: "qrc:/icons/close.png"
        }
        ToolBtn {
            id: _addSplit
            text: qsTr("Add split")
            width: parent.height
            height: parent.height
            anchors.right: parent.right
            flat: true
            icon: "qrc:/icons/split-horizontal.png"
            onClicked: {
                menu.open();
            }
        }
    }

    Menu {
        id: menu
        MenuItem {
            text: qsTr("Close")
        }
        MenuItem {
            text: qsTr("Add new")
        }
        MenuSeparator {
        }
        Repeater {
            model: header.model
            delegate: Action {
                text: caption
            }
        }
    }

    function indexByName(name)
    {
        for(var i = 0; i < header.model.length(); ++i){
            var plug = header.model.value(i);
            if (plug.name === name)
                return i;
        }
        return 0;
    }

    function itemByName(name)
    {
        if (!header.model)
            return null;

        for(var i = 0; i < header.model.length(); ++i){
            var plug = header.model.value(i);
            if (plug.name === name)
                return plug;
        }
        return null;
    }

    Component {
        id: addMenuItem
        MenuItem {

        }
    }

    Component.onCompleted: {
        header.model = qnoto.pluginsModel("qNoto.ToolPlugin");
    }

    onSelectedPluginChanged: {
        if (!selectedPlugin)
            return;

        title.text = selectedPlugin ? selectedPlugin.caption : "----"
    }
}
