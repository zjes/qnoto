import QtQuick 2.10
import QtQuick.Layouts 1.3

Item {
    id: splitView

    QtObject {
        id: type
        property int vertical: 1
        property int horizontal: 2
    }

    property bool lock: false
    property var orientation: type.horizontal

    property Component splitHDelegate : MouseArea {
        id: _split
        property var leftItem
        property var rightItem
        property var stPoint: null

        height: parent.height
        width: 5

        anchors.left: leftItem.right
        hoverEnabled: true
        cursorShape: Qt.SplitHCursor

        Rectangle {
            color: "#000000"
            border.color: "#000000"
            height: parent.height
            width: 1
            x: 2
        }

        onPressed: {
            stPoint = mouseX;
        }

        onMouseXChanged: {
            if (!pressed)
                return;

            leftItem.width += mouseX - stPoint
            stPoint = mouseX
        }
        Component.onCompleted: {
            rightItem.anchors.left = _split.right
        }
    }

    property Component splitVDelegate : MouseArea {
        id: _vsplit
        property var topItem
        property var bottomItem
        property var stPoint: null

        height: 5
        width: parent.width

        anchors.top: topItem.bottom
        hoverEnabled: true
        cursorShape: Qt.SplitVCursor

        Rectangle {
            color: "#000000"
            border.color: "#000000"
            height: 1
            width: parent.width
            y: 2
        }

        onPressed: {
            stPoint = mouseY;
        }

        onMouseXChanged: {
            if (!pressed)
                return;

            topItem.height += mouseY - stPoint
            stPoint = mouseY
        }
        Component.onCompleted: {
            bottomItem.anchors.top = _vsplit.bottom
        }
    }

    onChildrenChanged: {
        if (lock)
            return;

        if (children.length > 1){
            lock = true
            if (orientation == type.horizontal){
                splitHDelegate.createObject(splitView, {
                   "leftItem": children[children.length-2],
                   "rightItem": children[children.length-1]
                })
            } else {
                splitVDelegate.createObject(splitView, {
                   "topItem": children[children.length-2],
                   "bottomItem": children[children.length-1]
                })
            }
            lock = false
        }
    }
}
