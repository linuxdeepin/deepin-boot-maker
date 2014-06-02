import QtQuick 2.1
import Deepin.Widgets 1.0

Item {
    width: label.width + 8
    height: label.height + 6

    property alias text: label.text
    property alias fontSize: label.font.pixelSize
    property alias hasUnderline: label.font.underline

    signal clicked

    DLabel {
        id: label
        color: "#b4b4b4"
        font.pixelSize: 12
        anchors.centerIn: parent
    }

    states: [
        State {
            name: "hovered"
            PropertyChanges { target: label; color: "#0090ff" }
            PropertyChanges { target: mouseArea; cursorShape: Qt.PointingHandCursor }
        },
        State {
            name: "pressed"
            PropertyChanges { target: label; color: "#006ec3" }
            PropertyChanges { target: mouseArea; cursorShape: Qt.PointingHandCursor }
        }
    ]

    transitions: [
        Transition {
            from: ""
            to: "hovered"
            ColorAnimation { target: label; duration: 100}
        },
        Transition {
            from: "hovered"
            to: ""
            ColorAnimation { target: label; duration: 100}
        }
    ]

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        width: parent.width
        height: parent.height

        onEntered: parent.state = "hovered"
        onExited: parent.state = ""
        onPressed: parent.state = "pressed"
        onReleased: containsMouse ? parent.state = "hovered" : parent.state = ""
        onClicked: parent.clicked()
    }
}
