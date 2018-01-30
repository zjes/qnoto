import QtQuick 2.10
import QtQuick.Controls 2.3
import Controls 1.0
import Utils 1.0

Rectangle {

    color: "#ffffff"
    border.color: "#dedede"
    property var modifiedFiles: []

    ListView {
        id: list
        anchors.fill: parent

        delegate: MouseArea {
            height: 25
            width: list.lineWidth
            propagateComposedEvents: true
            hoverEnabled: true
            Rectangle {
                id: hover
                anchors.fill: parent
                visible: false
                radius: 4
                border.color: "lightsteelblue"
                color: "transparent"
            }

            Item {
                anchors.fill: parent

                Image {
                    id: icon
                    width: parent.height
                    height: parent.height
                    sourceSize.width: 22
                    sourceSize.height: 22
                    anchors.left: parent.left
                    source: "image://fileicon/"+encodeURIComponent(modelData.fileName)
                }

                Label {
                    anchors.left: icon.right
                    anchors.right: close.left
                    anchors.margins: 5
                    height: parent.height
                    text: FileInfo.filename(modelData.fileName)+(modelData.modified ? "*" : "")
                    verticalAlignment: Text.AlignVCenter
                }

                ToolButton {
                    id: close
                    anchors.right: parent.right
                    icon.source: "qrc:/icons/std/window-close.png"
                    width: parent.height
                    height: parent.height
                    visible: false
                    onClicked: {
                        fileHandler.close(modelData.fileName)
                    }
                }
            }
            onClicked: {
                list.currentIndex = index
                fileHandler.activate(modelData.fileName);
            }
            onContainsMouseChanged: {
                hover.visible = containsMouse
                close.visible = containsMouse
            }
        }
    }

    Connections {
        target: fileHandler

        onActivated: {
            if (!list.model)
                return;

            var found = list.model.findIndex(function(mf){
                return file === mf.fileName;
            });

            if (found >= 0)
                list.currentIndex = found;
        }

        onFilesChanged: {
            var items = fileHandler.files;
            var model = []
            for(var f in items){
                model.push({
                    "fileName": items[f],
                    "modified": modifiedFiles.indexOf(items[f]) >= 0
                });
            }
            model.sort(function(l, r){
                var nl = FileInfo.filename(l.fileName);
                var rl = FileInfo.filename(r.fileName);
                if (nl === rl)
                    return 0;
                if (nl < rl)
                    return -1;
                return 1;
            });
            list.model = model;
        }

        onModified: {
            if (modified){
                if (modifiedFiles.indexOf(file) == -1)
                    modifiedFiles.push(file);
            } else {
                var index = modifiedFiles.indexOf(file);
                modifiedFiles.splice(index, 1);
            }
            if (list.model){
                var mod = list.model;
                var findex = mod.findIndex(function(f){
                    return f.fileName === file;
                });
                if (findex >= 0){
                    mod[findex].modified = modified;
                    list.model = mod;
                }
            }
        }
    }
}
