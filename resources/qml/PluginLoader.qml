import QtQuick 2.9

MouseArea {
    property var plugin: null
    Loader {
        anchors.fill: parent
        focus: true
        id: _loader
        activeFocusOnTab: true
        sourceComponent: plugin ? qnoto.loadPlugin(_loader, plugin) : null
    }
    onClicked: {
        _loader.forceActiveFocus();
    }
}
