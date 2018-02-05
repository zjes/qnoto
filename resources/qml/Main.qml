import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Controls 1.0

ApplicationWindow {
    id    : mainWindow
    title : qsTr("qNoto")
    width : 1000
    height: 800

    /*menuBar: MenuBar {
        id: mainMenu
        Menu {
            id: fileMenu
            title: qsTr("&File")
        }
        Menu {
            id: editMenu
            title: qsTr("&Edit")
        }
        Menu {
            id: windowMenu
            title: qsTr("&Window")
        }
        Menu {
            id: settingsMenu
            title: qsTr("&Settings")
        }
    }*/

    SplitView {
        anchors.fill: parent
        LeftPlace {
            width: 250
            height: parent.height
        }
        Editors {
            anchors.right: parent.right
            height: parent.height
        }
    }
}
