import QtQuick 2.1

Rectangle {
    id: delegate

    width: delegate.GridView.view.cellWidth
    height: delegate.GridView.view.cellHeight
    color: Qt.rgba(0, 0, 0, 0)

    property int centerSpacing: 3
    property int verticalPadding: 3
    property int horizontalPadding: 10

    property color fontNormalColor: "#898989"
    /* property color fontHoverColor: "white" */
    property color fontPressedColor: "#19A9F9"
    property color bgNormalColor: Qt.rgba(0, 0, 0, 0)
    property color bgPressedColor: Qt.rgba(0, 0, 0, 0.4)

    property bool pressed: false
    property int delegateIndex: index

    function select() {
        if (delegate.GridView.view.singleSelectionMode == true) {
            delegate.GridView.view.clear()
        }

        if (!delegate.pressed) {
            /* text */
            txt.color = delegate.fontPressedColor
            /* background */
            contentBox.color = delegate.bgPressedColor

            delegate.GridView.view.selectedIndexs.push(index)
            delegate.GridView.view.selectedItems.push(delegate)
            delegate.pressed = true
        }
    }

    function deselect() {

        if (delegate.pressed) {
            /* text */
            txt.color = delegate.fontNormalColor
            /* background */
            contentBox.color = delegate.bgNormalColor

            var target_index_index = delegate.GridView.view.selectedIndexs.indexOf(index)
            var target_item_index = delegate.GridView.view.selectedItems.indexOf(delegate)

            delegate.GridView.view.selectedIndexs.splice(target_index_index, 1)
            delegate.GridView.view.selectedItems.splice(target_item_index, 1)
            delegate.pressed = false
        }
    }

    Component.onCompleted: {
        if ((typeof selected != "undefined") && selected) {
            delegate.select()
        }
    }

    Rectangle {
        id: contentBox
        radius: 4
        color: bgNormalColor
        width: horizontalPadding * 2 + txt.implicitWidth
        /* width: delegate.GridView.view.viewWidth */
        height: verticalPadding * 2 + txt.height + centerSpacing

        Text {
            id: txt

            color: fontNormalColor
            text: label
            font.pixelSize: 12

            anchors.centerIn: parent
        }

        anchors.centerIn: parent

    }

    MouseArea {
        anchors.fill: parent

        onPressed: {
            if (!delegate.pressed) {
                select()
                delegate.GridView.view.select(index, delegate)
            } else {
                if (delegate.GridView.view.singleSelectionMode == true && delegate.GridView.view.selectedIndexs.length == 1) {
                    return
                }
                deselect()
                delegate.GridView.view.deselect(index, delegate)
            }
        }
    }
}