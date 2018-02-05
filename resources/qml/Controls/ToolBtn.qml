import QtQuick 2.9
import QtQuick.Controls 2.2

ToolButton {
    property var icon

    contentItem: Image {
        source: icon
    }
}
