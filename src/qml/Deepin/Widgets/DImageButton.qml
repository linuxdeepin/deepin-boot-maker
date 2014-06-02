import QtQuick 2.1

Item {
    id: button
    
    property url normal_image
    property url hover_image
    property url press_image

    property alias mouseArea: mouseArea
    
    signal clicked

    property bool pressed: state == "pressed"

    states: [
        State {
            name: "hovered"
            PropertyChanges { target: image; source: button.hover_image }
        },
        State {
            name: "pressed"
            PropertyChanges { target: image; source: button.press_image }
        }
    ]
    
    width: image.width;    height: image.height
    
    Image {
        id: image
        source: normal_image
    }
    
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: { button.state = "hovered" }
        onExited: { button.state = "" }
        onPressed: { button.state = "pressed" }
        onReleased: { button.state = mouseArea.containsMouse ? "hovered" : ""}
        onClicked: button.clicked()
    }
}
