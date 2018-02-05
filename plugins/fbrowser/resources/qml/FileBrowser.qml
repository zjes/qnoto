import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Controls 1.0

Rectangle {

    color: "#ffffff"
    border.color: "#dedede"

    Pane {
        id: _header
        anchors.left: parent.left
        anchors.right: parent.right
        height: scroll.contentWidth > scroll.width ? 47 : 37
        padding: 1

        ScrollView {
            id: scroll
            anchors.fill: parent
            clip: true
            Row {
                height: 35
                spacing: 2
                Repeater {
                    id: curPath
                    model: fileBrowser.path
                    delegate: Row {
                        height: parent.height
                        spacing: 2
                        Label {
                            text: name
                            MouseArea {
                                anchors.fill: parent
                                hoverEnabled: true
                                onClicked: {
                                    fileBrowser.chdir(path)
                                }
                                onContainsMouseChanged: {
                                    parent.color = containsMouse ? "lightsteelblue" : "black";
                                }
                            }
                            verticalAlignment: Text.AlignVCenter
                            height: parent.height
                            font.pixelSize: 12
                            font.bold: true
                        }
                        Label {
                            text: "/"
                            height: parent.height
                            verticalAlignment: Text.AlignVCenter
                            font.pixelSize: 12
                        }
                    }
                }
            }
        }
    }

    ListView {
        id: list
        anchors.top: _header.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 4
        model: fileBrowser.entries

        delegate: MouseArea {
            height: 25
            width: list.lineWidth
            Row {
                anchors.fill: parent
                spacing: 5
                Image {
                    width: 22
                    height: 22
                    sourceSize.width: 22
                    sourceSize.height: 22
                    source: "image://fileicon/"+encodeURIComponent(path)
                }

                Label {
                    height: parent.height
                    text: name
                    verticalAlignment: Text.AlignVCenter
                }
                focus: true
            }

            onClicked: {
                list.currentIndex = index
                mouse.accepted = false
            }

            onDoubleClicked: {
                list.currentIndex = index
                mouse.accepted = false;
                if (isDir){
                    fileBrowser.chdir(path);
                    mouse.accepted = true;
                } else {
                    fileHandler.activate(path);
                    mouse.accepted = true;
                }
            }
        }
    }
}
