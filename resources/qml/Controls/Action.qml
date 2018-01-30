import QtQuick 2.10
import QtQuick.Controls 2.3
import Utils 1.0

Action {
    id: action
    property string shortcutText: shortcut ? Utils.shortcutText(shortcut, shortcut) : ""
    property int    modifiers   : shortcut ? Utils.shortcutModifiers(shortcut, shortcut) : 0
    property int    key         : shortcut ? Utils.shortcutKey(shortcut, shortcut) : 0
}
