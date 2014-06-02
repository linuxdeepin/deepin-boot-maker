import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Rectangle {
    id: header

    property alias leftLoader: leftArea
    property alias rightLoader: rightArea
    property int leftMargin: 15
    property int rightMargin: 15
    property var dconstants: DConstants {}

    height: 30
    width: parent.width
    color: dconstants.bgColor

    Loader {
        id: leftArea
        anchors.left: parent.left
        anchors.leftMargin: leftMargin
        anchors.verticalCenter: parent.verticalCenter
    }

    Loader {
        id: rightArea
        anchors.right: parent.right
        anchors.rightMargin: rightMargin
        anchors.verticalCenter: parent.verticalCenter
    }
}
