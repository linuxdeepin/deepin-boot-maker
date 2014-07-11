import QtQuick 2.1

Rectangle{
    id: textButton
    width: buttonHeader.width + buttonMiddle.width + buttonTail.width
    height: buttonHeader.height
    color: "transparent"
    property int minMiddleWidth: 50

    property alias text: title.text
    property alias textColor: title.color
    signal clicked

    QtObject { //enumeration for button image
        id: buttonImage
        property string headerNormal: "images/button_left_normal.png"
        property string headerPress: "images/button_left_press.png"
        property string middleNormal: "images/button_center_normal.png"
        property string middlePress: "images/button_center_press.png"
        property string tailNormal: "images/button_right_normal.png"
        property string tailPress: "images/button_right_press.png"
    }

    Row {
        anchors.top: parent.top
        anchors.left: parent.left

        Image{
            id: buttonHeader
            source: buttonImage.headerNormal
        }

        Image {
            id: buttonMiddle
            source: buttonImage.middleNormal
            width: title.width + 8 < minMiddleWidth ? minMiddleWidth : title.width + 8

            Text{
                id: title
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                color: "#b4b4b4"
                font.pixelSize: 12
            }

            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                width: parent.width
                height: parent.height
                onPressed: {
                    buttonHeader.source = buttonImage.headerPress
                    buttonMiddle.source = buttonImage.middlePress
                    buttonTail.source = buttonImage.tailPress
                }
                onReleased: {
                    buttonHeader.source = buttonImage.headerNormal
                    buttonMiddle.source = buttonImage.middleNormal
                    buttonTail.source = buttonImage.tailNormal
                }
                onClicked: {
                    textButton.clicked()
                }
            }
        }

        Image{
            id: buttonTail
            source: buttonImage.tailNormal
        }
    }
}
