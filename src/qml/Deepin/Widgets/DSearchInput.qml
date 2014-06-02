import QtQuick 2.1

DTextInput {
    id: searchInput
    textInputRightMargin: clearImageBox.width
    textInputLeftMargin: searchImageBox.width

    Item {
        id: searchImageBox
        width: searchImage.width + 4
        height: parent.height

        Image {
            id: searchImage
            anchors.centerIn: parent
            source: "images/search_white.png"
        }
    }

    Item {
        id: clearImageBox
        anchors.right: parent.right
        width: clearImage.width + padding * 2
        height: parent.height
        property int padding: 3

        DOpacityImageButton {
            id: clearImage
            visible: searchInput.text != ""
            source: "images/clear_white.png"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            anchors.verticalCenterOffset: 1
            onClicked: {
                searchInput.text = ""
            }
        }
    }
}
