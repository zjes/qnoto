import QtQuick 2.10
import QtQuick.Controls 2.3

ListView {
    id: list
    anchors.margins: 4
    focus: true
    keyNavigationEnabled: true
    property bool vertScrollVisible: list.height < list.contentHeight
    property bool horizScrollVisible: list.width < list.contentWidth
    property int lineWidth: vertScrollVisible ? width - 10 : width

    ScrollBar.vertical: ScrollBar {
        interactive: true
        orientation: Qt.Vertical
        policy: vertScrollVisible ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
    }
    ScrollBar.horizontal: ScrollBar {
        interactive: true
        orientation: Qt.Horizontal
        policy: horizScrollVisible ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
    }
    clip: true
    highlightMoveDuration: 100

    highlight: Rectangle {
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#efefef" }
            GradientStop { position: 0.5; color: list.activeFocus ? "#dedeff" : "#efefef" }
            GradientStop { position: 1.0; color: "#efefef" }
        }
        radius: 4
        border.color: "lightsteelblue"
    }
}
