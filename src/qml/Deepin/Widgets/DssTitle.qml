import QtQuick 2.0

DBaseLine {
    id: dssTitle
    width: parent.width
    height: 48

    signal titleClicked
    property string text: ""

    property var dconstants: DConstants {}

    leftLoader.sourceComponent: DssH1 {
        id: title
        font.bold: true
        color: "white"
        text: dssTitle.text

        MouseArea {
            anchors.fill: parent
            onClicked: dssTitle.titleClicked()
        }
    }
}
