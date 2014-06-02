import QtQuick 2.1
import QtGraphicalEffects 1.0

Rectangle {
    id: round_image

    property int roundRadius: 50
    property url imageSource: ""
    property int borderWidth: 3
    property color borderColor: "white"
    property int glowRadius: 0
    property color glowColor: "#19A9F9"

    property bool checkSupport: false

    width: (roundRadius + borderWidth) * 2
    height: (roundRadius + borderWidth) * 2
    radius: roundRadius + borderWidth
    color: borderColor
    state: "normal"

    signal clicked
    signal entered
    signal exited
    signal imageLoadError

    states: [
        State {
            name: "normal"
            PropertyChanges {
                target: glow
                visible: false
            }
            PropertyChanges {
                target: round_image
                borderColor: "white"
            }
        },
        State {
            name: "checked"
            PropertyChanges {
                target: glow
                visible: true
            }
            PropertyChanges {
                target: round_image
                borderColor: "#19A9F9"
            }
        }
    ]

    Rectangle {
        id: round_bg
        radius: round_image.roundRadius

        width: round_item.width
        height: round_item.height

        anchors.centerIn: parent
    }

    RectangularGlow {
        id: glow

        spread: 0.5
        color: round_image.glowColor
        cornerRadius: round_image.roundRadius + round_image.glowRadius
        glowRadius: round_image.glowRadius

        anchors.fill: round_bg
    }

    DRoundItem {
        id: round_item

        width: radius * 2
        height: radius * 2
        radius: parent.roundRadius

        Image {
            source: round_image.imageSource

            onStatusChanged: {
                if (status == Image.Error) {
                    round_image.imageLoadError()
                }
            }
        }

        anchors.centerIn: parent
    }

    MouseArea {
        hoverEnabled: true
        anchors.fill: parent

        onClicked: {
            parent.state = parent.checkSupport ? (parent.state == "normal" ? "checked" : "normal") : "normal"
            parent.clicked()
        }
        
        onEntered: parent.entered()
        onExited: parent.exited()
    }
}
