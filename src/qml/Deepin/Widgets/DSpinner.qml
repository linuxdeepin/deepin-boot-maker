import QtQuick 2.0

DTextInput {
    property int min: 0
    property int max: 65535
    property int step: 1

    property int value: parseInt(textInput.text)

    function setValue(i){
        textInput.text = i
    }

    textInput.validator: IntValidator{bottom: min; top: max;}

    textInput.anchors.rightMargin: 3 + buttonBox.width

    Connections{
        target: textInput
        onTextChanged: {
            if(textInput.text == ""){
                textInput.text = min
            }
            if (parseInt(textInput.text) < min) {
                textInput.text = min
            }
            if (parseInt(textInput.text) > max) {
                textInput.text = max
            }
        }
    }

    Connections {
        target: increaseButton.mouseArea
        onPressAndHold: {
            holdTimer.isIncrease = true
            holdTimer.restart()
        }
        onReleased: {
            holdTimer.stop()
        }
    }

    Connections {
        target: decreaseButton.mouseArea
        onPressAndHold: {
            holdTimer.isIncrease = false
            holdTimer.restart()
        }
        onReleased: {
            holdTimer.stop()
        }
    }

    Timer {
        id: holdTimer
        repeat: true
        interval: 50
        property bool isIncrease: true
        onTriggered: {
            if(isIncrease){
                increase()
            }
            else{
                decrease()
            }
        }
    }

    function increase(){
        if(value <= max - step){
            setValue(value + step)
        }
        else{
            setValue(max)
        }
    }

    function decrease(){
        if(value >= min + step){
            setValue(value - step)
        }
        else{
            setValue(min)
        }
    }

    Row {
        id: buttonBox
        parent: textInputBox
        height: parent.height
        anchors.right: parent.right

        Rectangle {
            width: 1
            height: parent.height
            color: Qt.rgba(1, 1, 1, 0.1)
        }

        Item {
            width: increaseButton.width
            height: parent.height

            Rectangle {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: increaseButton.pressed ? Qt.rgba(0, 0, 0, 0.05) : Qt.rgba(1, 1, 1, 0.05)
                    }
                    GradientStop {
                        position: 1.0
                        color: increaseButton.pressed ? Qt.rgba(0, 0, 0, 0) : Qt.rgba(1, 1, 1, 0)
                    }
                } 
            }

            DImageButton {
                id: increaseButton
                anchors.verticalCenter: parent.verticalCenter
                normal_image: "images/spinner_increase_normal.png"
                hover_image: "images/spinner_increase_hover.png"
                press_image: "images/spinner_increase_press.png"

                onClicked: increase()
            }
        }

        Rectangle {
            width: 1
            height: parent.height
            color: Qt.rgba(1, 1, 1, 0.1)
        }

        Item {
            width: decreaseButton.width
            height: parent.height

            Rectangle {
                anchors.fill: parent
                radius: 3
                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: decreaseButton.pressed ? Qt.rgba(0, 0, 0, 0.05) : Qt.rgba(1, 1, 1, 0.05)
                    }
                    GradientStop {
                        position: 1.0
                        color: decreaseButton.pressed ? Qt.rgba(0, 0, 0, 0) : Qt.rgba(1, 1, 1, 0)
                    }
                } 
            }

            DImageButton{
                id: decreaseButton
                anchors.verticalCenter: parent.verticalCenter
                normal_image: "images/spinner_decrease_normal.png"
                hover_image: "images/spinner_decrease_hover.png"
                press_image: "images/spinner_decrease_press.png"

                onClicked: decrease()
            }
        }
    }
}
