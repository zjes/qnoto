import QtQuick 2.10
import QtQuick.Controls 2.3
import Utils 1.0

MenuItem {
    id: cnt

    property alias shortcut: act.shortcut
    property var shortcutText: Utils.shortcutText(act.shortcut)

    contentItem: Label {
        text: cnt.text
        color: "red"
    }

    action: Action {
        id: act
        onTriggered: console.log("aaaa")
    }
}
