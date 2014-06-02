import QtQuick 2.1
import QtGraphicalEffects 1.0

Item {
    id: round_item
    width: 100
    height: 100

    property int radius: 50
    property color bgColor: "#1A1A1B"
    default property alias source: target.sourceComponent

    Loader {
        id: target
    }

    Rectangle {
        id: mask

        color: round_item.bgColor
        radius: round_item.radius
        smooth: true
        antialiasing: true

        anchors.fill: parent
    }

    OpacityMask {
        anchors.fill: parent

        source: target
        maskSource: mask

        Component.onCompleted: {
            target.visible = false
        }
    }
}
