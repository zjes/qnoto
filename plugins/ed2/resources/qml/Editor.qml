import QtQuick 2.10
import QtQuick.Controls 2.3
import CodeEditor 1.0

Rectangle {
    id: editorView
    anchors.fill: parent

    //property alias fileName: instance.fileName
    //readonly property alias isModified: instance.isModified

    property bool vertScrollVisible: flickable.height < flickable.contentHeight
    property bool horizScrollVisible: flickable.width < flickable.contentWidth
    //color: txteditor.theme.backgroud()

    Flickable {
        id: flickable
        anchors.fill: parent
        //contentHeight: txteditor.paintedHeight
        //contentWidth: txteditor.paintedWidth
        clip: true
        rightMargin: vertScrollVisible ? 20 : 0
        bottomMargin: horizScrollVisible ? 20 : 0

        ScrollBar.vertical: ScrollBar{
            interactive: true
            orientation: Qt.Vertical
            policy: vertScrollVisible ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
            background: Rectangle {
                color: "#dedede"
            }
            contentItem: Rectangle {
                implicitWidth: 10
                radius: width / 2
            }
        }

        ScrollBar.horizontal: ScrollBar{
            orientation: Qt.Horizontal
            policy: horizScrollVisible ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
            interactive: true
            background: Rectangle {
                color: "#dedede"
            }
            contentItem: Rectangle {
                implicitHeight: 10
                radius: height / 2
            }
        }

        CodeEditor {
            id: txteditor
            text: "some text"
            width: 100
            height: 100
//            text: instance.text
//            focus: true
//            font.family: "Consolas"
//            font.pointSize: 11
//            activeFocusOnPress: true
//            selectByKeyboard: true
//            selectByMouse: true
//            onCursorRectangleChanged: flickable.ensureVisible(cursorRectangle)
//            activeFocusOnTab: true
//            color: instance.theme.color()
//            rightPadding: vertScrollVisible ? 20 : 0
//            bottomPadding: horizScrollVisible ? 20 : 0
        }
    }

    onActiveFocusChanged: {
        if (activeFocus)
            txteditor.forceActiveFocus();
    }

    Component.onCompleted: {
        console.log("Ed2 completed");
    }
}
