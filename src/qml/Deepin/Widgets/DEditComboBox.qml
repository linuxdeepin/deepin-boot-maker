import QtQuick 2.1

DTextInput {
    id: editComboBox
    
    signal clicked
    
    textInput.anchors.rightMargin: 3 + buttonBox.width
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
