import QtQuick 2.10
import QtQuick.Controls 2.3
import Controls 1.0
import Utils 1.0

Popup {
    id: switcher
    property var forward: null
    property var backward: null
    signal selected(string file)

    width: contentItem.implicitWidth
    height: contentItem.implicitHeight

    x: (parent.width - width)/2
    y: (parent.height - height)/2

    padding: 4

    contentItem: ListView {
        id: list
        anchors.fill: parent
        model: fileHandler.files

        implicitWidth: 200
        implicitHeight: model.length >= 10 ? 10 * (25+padding) : model.length*(25+padding)

        delegate: Row {
            height: 25
            width: list.lineWidth
            spacing: 5
            Image {
                width: 22
                height: 22
                sourceSize.width: 22
                sourceSize.height: 22
                source: "image://fileicon/"+encodeURIComponent(modelData)
            }

            Label {
                focus: true
                height: parent.height
                text: FileInfo.filename(modelData)
                verticalAlignment: Text.AlignVCenter
            }
        }
        Keys.onPressed: {
            if (event.key === forward.key && event.modifiers === forward.modifiers){
                next();
            } else {
                if (backward.key === Qt.Key_Tab){
                    if ((event.key === Qt.Key_Tab || event.key === Qt.Key_Backtab)  && event.modifiers === backward.modifiers){
                        prev();
                    }
                } else {
                    if (event.key === backward.key && event.modifiers === backward.modifiers)
                        prev();
                    }
            }
        }
        Keys.onReleased: {
            if (event.modifiers === 0)
                switcher.visible = false;
        }
    }

    onAboutToHide: {
        var sel = fileHandler.files[list.currentIndex];
        selected(sel);
    }

    function next()
    {
        if (!Utils.globalModifiers()){
            var index = 0;
            if (index < list.model.length){
                var sel = fileHandler.files[index];
                selected(sel);
            }
            return;
        }

        if(!visible){
            list.currentIndex = 0;
            open();
        }
        list.forceActiveFocus();
        if (list.currentIndex >= list.model.length-1)
            list.currentIndex = 0;
        else
            ++list.currentIndex;
    }

    function prev()
    {
        if (!Utils.globalModifiers()){
            var index = list.model.length-1;
            if (index >= 0){
                var sel = fileHandler.files[index];
                selected(sel);
            }
            return;
        }

        if(!visible){
            list.currentIndex = list.model.length-1;
            open();
            list.forceActiveFocus();
        } else {
            if (list.currentIndex <= 0)
                list.currentIndex = list.model.length-1;
            else
                --list.currentIndex;
        }
    }
}
