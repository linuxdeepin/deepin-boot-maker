import QtQuick 2.1

Item {
    width: parent.width
    height: 2

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        width: parent.width
        height: 1
        color: "#120f10"
    }
    Rectangle {
        anchors.top: parent.top
        anchors.topMargin: 1
        anchors.left: parent.left
        width: parent.width
        height: 1
        color: "#303132"
    }
}
