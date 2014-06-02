import QtQuick 2.1

Item {
    id: button

    property var statusImageList: {}
    property string currentStatus: "undefined"

    property url normal_image: statusImageList[currentStatus][0]
    property url hover_image: statusImageList[currentStatus][1]
    property url press_image: statusImageList[currentStatus][2]

    signal clicked
    
    width: image.width
    height: image.height
    
    Image {
        id: image
        source: normal_image
    }
    
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: { image.source = hover_image }
        onExited: { image.source = normal_image }
        onPressed: { image.source = press_image }
        onReleased: {
            button.clicked()
            image.source= mouseArea.containsMouse ? hover_image : normal_image
        }
    }
}
