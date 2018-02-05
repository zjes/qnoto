import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import Utils 1.0
import Controls 1.0

Column {
    id: manager

    Frame {
        id: _header
        height: 25
        width: parent.width
        Label {
            id: header
            anchors.fill: parent
            verticalAlignment: Label.AlignVCenter
            horizontalAlignment: Label.AlignLeft
        }
    }

    StackLayout {
        id: editors
        property var items: []
        width: parent.width
        height: parent.height - _header.height
        onCurrentIndexChanged: {
            var cur = editors.itemAt(editors.currentIndex);
            if (cur){
                header.text = cur.fileName
            } else {
                header.text = "";
            }
        }
    }

    Connections {
        target: fileHandler
        onActivated: {
            fileActivated(file);
        }
    }

    Component.onCompleted: {
        if (qnoto.settings.restoreState){
            var files = [];
            for(var i = qnoto.settings.files.length-1; i >= 0; --i){
                files.push(qnoto.settings.files[i]);
            }
            for(var i in files){
                fileHandler.activate(files[i]);
            }
        }
        updateMenu();
    }

    function fileActivated(fileName)
    {
        var foundIndex = find(fileName);
        if (foundIndex >= 0){
            editors.currentIndex = foundIndex;
        } else {
            var comp = qnoto.loadPlugin(manager, "qNoto.Editor.DefaultEditor");
            if (!comp){
                console.warn("Cannot load editor");
            } if (comp.status === Component.Ready){
                finishCreation(comp, fileName);
            } if (comp.status === Component.Error){
                console.warn("Cannot load editor:", comp.errorString());
            } else {
                comp.statusChanged.connect(function(){
                    finishCreation(comp, fileName);
                });
            }
        }
        updateMenu();
        current().forceActiveFocus();
    }

    function finishCreation(component, fname)
    {
        console.log(component, component.status);
        if (component.status === Component.Ready) {
            var obj = component.createObject(editors);
            if (obj === null) {
                console.log("Error creating object");
            } else {
                console.log("Add editor");
                obj.load(fname);
                editors.items.push(obj);
                editors.currentIndex = find(fname);
            }
        } else if (component.status === Component.Error) {
            console.log("Error loading component:", component.errorString());
        }
    }

    function find(fileName)
    {
        var foundIndex = -1;
        for(var i = 0; i < editors.count; ++i){
            var it = editors.itemAt(i);
            if (it.fileName !== fileName)
                continue;
            foundIndex = i;
            break;
        }
        return foundIndex;
    }

    function current()
    {
        return editors.itemAt(editors.currentIndex);
    }

    /*Action {
        id: fileNew
        text: qsTr("File new") + " ("+shortcutText+")"
        shortcut: StandardKey.New
        onTriggered: {
            console.log("file new")
        }
    }

    Action {
        id: fileOpen
        text: qsTr("File open...") + " ("+shortcutText+")"
        icon.name: "file-open"
        shortcut: StandardKey.Open
    }

    Action {
        id: quit
        text: qsTr("Quit")
        icon.name: "exit"
        shortcut: StandardKey.Quit
    }

    Action {
        id: saveFile
        icon.name: "file-save"
        shortcut: StandardKey.Save
        enabled: current && current.isModified
    }

    Action {
        id: saveFileAs
        icon.name: "file-saveas"
        shortcut: StandardKey.SaveAs
        enabled: current
    }

    Action {
        id: saveFileAll
        icon.name: "file-save"
        text: qsTr("Save all")
        enabled: canSave()
        function canSave() {
            for(var i = 0; i < editors.count; ++i){
                var it = editors.itemAt(i);
                if (it.isModified)
                    return true;
            }
            return false;
        }
    }

    Action {
        id: closeFile
        icon.name: "file-close"
        shortcut: StandardKey.Close
        enabled: current
        onTriggered: {
            console.log("close file", Menu.contentItem)
        }
    }

    Action {
        id: closeFileAllExcept
        icon.name: "file-close"
        enabled: current
    }

    Action {
        id: closeFileAll
        icon.name: "file-close"
        text: qsTr("Close all")
    }*/

    Action {
        id: switchWindowForward
        sequence: StandardKey.NextChild
        //text: qsTr("Previous document in history") + "("+shortcutText+")"
        enabled: fileHandler.files.length > 0
        Component.onCompleted: {
            //windowMenu.addAction(switchWindowForward)
        }
        onActivated: {
            wndSwitch.next();
        }
    }

    Action {
        id: switchWindowBackward
        sequence: "Ctrl+Shift+Tab"
        //text: qsTr("Next document in history") + "("+shortcutText+")"
        enabled: fileHandler.files.length > 0
        Component.onCompleted: {
            //windowMenu.addAction(switchWindowBackward)
        }
        onActivated: {
            wndSwitch.prev();
        }
    }

    WindowSwitcher {
        id: wndSwitch
        forward: switchWindowForward
        backward: switchWindowBackward
        onSelected: {
            if (file != "")
                fileHandler.activate(file);
        }
    }

    function createMenu()
    {
        /*while(fileMenu.count){
            fileMenu.takeItem(0);
        }

        fileMenu.addAction(fileNew);
        fileMenu.addAction(fileOpen);
        if(editors.count){
            addSeparator(fileMenu);
            fileMenu.addAction(saveFile);
            fileMenu.addAction(saveFileAs);
            fileMenu.addAction(saveFileAll);
            addSeparator(fileMenu);
            fileMenu.addAction(closeFile);
            fileMenu.addAction(closeFileAll);
            fileMenu.addAction(closeFileAllExcept);
        }
        addSeparator(fileMenu);
        fileMenu.addAction(quit);*/
    }

    function updateMenu()
    {
//        createMenu();
//        if(editors.count){
//            var fileName = FileInfo.filename(current.fileName);
//            saveFile.text = qsTr("Save '%1'").arg(fileName)
//            saveFileAs.text = qsTr("Save '%1' as...").arg(fileName)
//            closeFile.text = qsTr("Close '%1'").arg(fileName)
//            closeFileAllExcept.text = qsTr("Close all except '%1'").arg(fileName)
//        }
    }

    function addSeparator(menu)
    {
        menu.addItem(Qt.createQmlObject("import QtQuick.Controls 2.3; MenuSeparator{}", menu));
    }
}
