import QtQuick 2.1

DTextInput {
    id: editComboBox
    textInput.anchors.rightMargin: 3 + buttonBox.width
    
    property alias menu: menu

    property var parentWindow
    property var labels
    property var values
    property int selectIndex: -1
    onSelectIndexChanged: menu.currentIndex = selectIndex

    signal clicked
    signal menuSelect(int index)

    Component.onCompleted: {
        if(selectIndex != -1){
            if (values) {
                text = values[selectIndex]
            } else {
                text = menu.labels[selectIndex]
            }
            menu.currentIndex = selectIndex
        }
    }

    DMenu {
        id: menu
        parentWindow: editComboBox.parentWindow
        labels: editComboBox.labels
        onMenuSelect: {
            editComboBox.menuSelect(index)
            selectIndex = index
            if (values) {
                editComboBox.text = values[selectIndex]
            } else {
                editComboBox.text = menu.labels[selectIndex]
            }
        }
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

    Item {
        id: buttonBox
        width: childrenRect.width
        height: parent.height
        anchors.right: parent.right
        z: 10

        Rectangle {
            width: 1
            height: parent.height
            color: Qt.rgba(1, 1, 1, 0.1)
        }

        Rectangle {
            width: parent.width - 1
            height: parent.height
            anchors.right: parent.right
            radius: 3
            gradient: Gradient {
                GradientStop {
                    position: 0.0
                    color: dropDownButton.pressed ? Qt.rgba(0, 0, 0, 0.05) : Qt.rgba(1, 1, 1, 0.05)
                }
                GradientStop {
                    position: 1.0
                    color: dropDownButton.pressed ? Qt.rgba(0, 0, 0, 0) : Qt.rgba(1, 1, 1, 0)
                }
            } 
        }

        Item {
            width: 24
            height: parent.height
            DImageButton {
                id: dropDownButton
                anchors.centerIn: parent
                normal_image: "images/arrow_down_normal.png"
                hover_image: "images/arrow_down_hover.png"
                press_image: "images/arrow_down_press.png"
                onClicked: editComboBox.clicked()
            }
        }
    }
}
