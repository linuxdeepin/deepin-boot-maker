import QtQuick 2.1

Item {
    id: arrowButton
    anchors.centerIn: parent
    width: image.width
    height: image.height

    property bool checked: false
    state: checked ? "up" : "down"

    signal clicked

    property string imageKey: "down"

    DOpacityImageButton {
        id: image
        anchors.centerIn: parent
        source: "images/arrow_" + imageKey + "_white.png"

        onClicked: {
            arrowButton.checked = !arrowButton.checked
            arrowButton.clicked()
        }

    }

    states:  [
        State {
            name: "down"
            PropertyChanges { target: arrowButton; imageKey: "down" }
        },
        State {
            name: "up"
            PropertyChanges { target: arrowButton; imageKey: "up" }
        }
    ]
}
