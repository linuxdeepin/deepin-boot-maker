import QtQuick 2.1

Component {
    Rectangle {
        id: rect
        radius: 3
        color: Qt.rgba(0, 0, 0, 0)
        width: lable.implicitWidth + ListView.view.leftRightPadding * 2
        height: ListView.view.buttonHeight

        property var delegateId: buttonId

        Text {
            id: lable

            text: buttonLabel
            color: index == rect.ListView.view.currentIndex ? rect.ListView.view.selectedFontColor : rect.ListView.view.unselectedFontColor
            font.pixelSize: rect.ListView.view.fontSize

            anchors.centerIn: parent
        }

        function select() {
            lable.color = rect.ListView.view.selectedFontColor
        }
        function deselect() {
            lable.color = rect.ListView.view.unselectedFontColor
        }

        Connections {
            target: rect.ListView.view
            onCurrentIndexChanged: {
                if (rect.ListView.view.currentIndex == index) {
                    rect.select()
                } else {
                    rect.deselect()
                }
            }
        }

        /* dynamically update the size of listview */
        Component.onCompleted: {
            if (index == 0) {
                rect.ListView.view.width = rect.width
            } else {
                rect.ListView.view.width += rect.width
            }
            rect.ListView.view.height = rect.height
        }

        MouseArea {
            anchors.fill: parent

            onReleased: {
                rect.ListView.view.selectItem(index)
            }
        }
    }
}
