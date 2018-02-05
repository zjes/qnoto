import QtQuick 2.9
import QtQuick.Controls 2.2
import Utils 1.0

Shortcut {
    id: action
    property string shortcutText: sequence ? Utils.shortcutText(sequence, sequence) : ""
    property int    modifiers   : sequence ? Utils.shortcutModifiers(sequence, sequence) : 0
    property int    key         : sequence ? Utils.shortcutKey(sequence, sequence) : 0
}
