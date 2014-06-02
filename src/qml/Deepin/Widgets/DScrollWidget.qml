import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

ScrollView {
    id: root
    frameVisible: false
    property int rightMargin
    Component.onCompleted: {
        viewport.anchors.rightMargin = 0
        __scroller.rightMargin = -3 + rightMargin
        flickableItem.interactive = true
    }

    style: ScrollViewStyle {
        handleOverlap: 0
        handle: Item {
            implicitWidth: 14
            implicitHeight: 26
            Rectangle {
                color: styleData.hovered ? Qt.rgba(66/255.0, 66/255.0, 70/255.0, 0.8) : Qt.rgba(100/255.0, 100/255.0, 100/255.0)
                anchors.fill: parent
                anchors.topMargin: 6
                anchors.leftMargin: 4
                anchors.rightMargin: 4
                anchors.bottomMargin: 6
                radius: 3
            }
        }
        scrollBarBackground: Item {
            implicitWidth: 14
            implicitHeight: 26
        }
        decrementControl: Item {}
        incrementControl: Item {}
    }
}
