import QtQuick 2.1
import QtGraphicalEffects 1.0

Item {
    id: root
    
    width: 55
    height: 24
    
    property bool checked: false
    property bool enabled: true

    state: { enabled ? checked ? "on" : "off" : "disabled" }

    signal clicked

    transitions: Transition {
        NumberAnimation { properties: "x"; duration: 150; easing.type: Easing.InOutQuad }
    }
    
    Rectangle {
        id: mask

        width: 55
        height: 24
        color: "transparent"

        Rectangle {
            width: 52
            height: 18
            radius: 9
            antialiasing: true

            anchors.centerIn: parent
        }

        anchors.centerIn: parent
    }

    Rectangle {
        id: inner

        width: 55
        height: 24
        color: "transparent"
        anchors.verticalCenter: parent.verticalCenter

        Image {
            id: inner_image

            anchors.verticalCenter: parent.verticalCenter
        }

        /* Desaturate { */
        /*     id: desaturate */
        /*     anchors.fill: inner_image */
        /*     source: inner_image */
        /*     desaturation: 1 */
        /* } */
    }

    OpacityMask {
        anchors.fill: mask
        source: ShaderEffectSource { sourceItem: inner; hideSource: true }
        maskSource: ShaderEffectSource { sourceItem: mask; hideSource: true }
    }

    states:  [
        State {
            name: "on"
            PropertyChanges { target: inner_image; x: 0; source: "images/switch_inner.svg" }
            PropertyChanges { target: mouse_area; enabled: true }
            /* PropertyChanges { target: desaturate; visible: false } */
        },
        State {
            name: "off"
            PropertyChanges { target: inner_image; x: -34; source: "images/switch_inner.svg" }
            PropertyChanges { target: mouse_area; enabled: true }
            /* PropertyChanges { target: desaturate; visible: false } */
        },
        State {
            name: "disabled"
            PropertyChanges { target: inner_image; x: inner_image.x; source: "images/switch_inner_insensitive.svg" }
            PropertyChanges { target: mouse_area; enabled: false }
            /* PropertyChanges { target: desaturate; visible: true } */
        }
    ]

    MouseArea {
        id: mouse_area
        anchors.fill: parent
        visible: enabled

        onClicked: {
            checked = !checked
            root.clicked()
        }
    }
}
