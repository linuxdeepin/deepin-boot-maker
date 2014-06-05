import QtQuick 2.1
import QtQuick.Window 2.1

Item {
    id: combobox
    width: Math.max(minMiddleWidth, parent.width)
    height: background.height

    property bool hovered: false
    property bool pressed: false

    property alias text: currentLabel.text
    property var labels

    signal clicked

    property var parentWindow

    property var menu: DMenu {
        parentWindow: combobox.parentWindow
        labels: combobox.labels
    }

    function showMenu(x, y, w) {
        menu.x = x - menu.frameEdge
        menu.y = y - menu.frameEdge
        menu.width = w + menu.frameEdge * 2
        menu.visible = true
    }

    onClicked: {
        var pos = mapToItem(null, 0, 0)
        var x = parentWindow.x + pos.x
        var y = parentWindow.y + pos.y + height
        var w = width
        showMenu(x, y, w)
    }

    QtObject {
        id: buttonImage
        property string status: "normal"
        property string header: "images/button_left_%1.png".arg(status)
        property string middle: "images/button_center_%1.png".arg(status)
        property string tail: "images/button_right_%1.png".arg(status)
    }

    property int minMiddleWidth: buttonHeader.width + downArrow.width + buttonTail.width

    Row {
        id: background
        height: buttonHeader.height
        width: parent.width

        Image{
            id: buttonHeader
            source: buttonImage.header
        }

        Image {
            id: buttonMiddle
            source: buttonImage.middle
            width: parent.width - buttonHeader.width - buttonTail.width
        }

        Image{
            id: buttonTail
            source: buttonImage.tail
        }
    }

    Rectangle {
        id: content
        width: buttonMiddle.width
        height: background.height
        anchors.left: parent.left
        anchors.leftMargin: buttonHeader.width
        anchors.verticalCenter: parent.verticalCenter
        color: Qt.rgba(1, 0, 0, 0)

        DssH2 {
            id: currentLabel
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width - downArrow.width
            elide: Text.ElideRight
        }

        Image {
            id: downArrow
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            source: hovered ? "images/arrow_down_hover.png" : "images/arrow_down_normal.png"
        }

    }

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            parent.hovered = true
        }

        onExited: {
            parent.hovered = false
        }

        onPressed: {
            parent.pressed = true
            buttonImage.status = "press"
        }
        onReleased: {
            parent.pressed = false
            parent.hovered = containsMouse
            buttonImage.status = "normal"
        }

        onClicked: {
            combobox.clicked()
        }
    }

}
