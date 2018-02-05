import QtQuick 2.9
import QtQuick.Controls 2.2
import DefaultEditor 1.0

Rectangle {
    id: editorView
    anchors.fill: parent

    property alias fileName: instance.fileName
    readonly property alias isModified: instance.isModified

    property bool vertScrollVisible: flickable.height < flickable.contentHeight
    property bool horizScrollVisible: flickable.width < flickable.contentWidth
    color: instance.theme.backgroud()

    Flickable {
        id: flickable
        anchors.fill: parent
        contentHeight: txteditor.paintedHeight
        contentWidth: txteditor.paintedWidth
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

        function ensureVisible(r)
        {
            if (contentX >= r.x)
                contentX = r.x;
            else if (contentX+width+rightMargin <= r.x+r.width)
                contentX = r.x+r.width-width+rightMargin;
            if (contentY >= r.y)
                contentY = r.y;
            else if (contentY+height+bottomMargin <= r.y+r.height)
                contentY = r.y+r.height-height+bottomMargin;
        }

        MouseArea {
            anchors.fill: parent
            propagateComposedEvents: true
            cursorShape: Qt.IBeamCursor
            onWheel: {
                var delta = txteditor.cursorRectangle.height * 2;
                if (wheel.angleDelta.y > 0){
                    if (flickable.contentY > delta)
                        flickable.contentY -= delta;
                    else
                        flickable.contentY = 0
                } else if (wheel.angleDelta.y < 0){
                    if (flickable.contentY + flickable.height < flickable.contentHeight)
                        flickable.contentY += delta;
                    else if (flickable.contentHeight - flickable.height > 0)
                        flickable.contentY = flickable.contentHeight - flickable.height;
                }
            }
        }

        TextEdit {
            id: txteditor
            text: instance.text
            focus: true
            font.family: "Consolas"
            font.pointSize: 11
            activeFocusOnPress: true
            selectByKeyboard: true
            selectByMouse: true
            onCursorRectangleChanged: flickable.ensureVisible(cursorRectangle)
            activeFocusOnTab: true
            color: instance.theme.color()
            rightPadding: vertScrollVisible ? 20 : 0
            bottomPadding: horizScrollVisible ? 20 : 0

            DefaultEditor
            {
                id: instance
                target: txteditor
            }

            Keys.onPressed: {
                if (event.key === Qt.Key_PageDown || event.key === Qt.Key_PageUp){
                    var cur = txteditor.cursorRectangle;
                    var relPos = txteditor.positionAt(cur.x, cur.y);

                    var delta = Math.ceil(flickable.height / cur.height) * cur.height;
                    if (event.key === Qt.Key_PageUp)
                        delta *= -1;

                    if (delta > 0){
                        if (flickable.contentY + delta < flickable.contentHeight - flickable.height)
                            flickable.contentY += delta;
                        else
                            flickable.contentY = flickable.contentHeight - flickable.height;
                    } else {
                        if (flickable.contentY + delta > 0)
                            flickable.contentY += delta;
                        else
                            flickable.contentY = 0;
                    }

                    txteditor.cursorPosition = txteditor.positionAt(cur.x, cur.y + delta);
                    flickable.ensureVisible(txteditor.cursorRectangle);
                }
            }
        }
    }

    onActiveFocusChanged: {
        if (activeFocus)
            txteditor.forceActiveFocus();
    }

    function load(fileName)
    {
        instance.fileName = fileName;
    }

    Component.onCompleted: {
        txteditor.forceActiveFocus();
    }
}
