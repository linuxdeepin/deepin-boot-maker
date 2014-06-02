import QtQuick 2.1
import Deepin.Widgets 1.0

Item {
    id: root
    width: 300
    height: 100

    property int leftPadding: 15
    property int moveDelta: 50
    property bool active: false
    property alias content: loader.sourceComponent
    
    signal expanded
    signal shrinked
    signal action
    
    function expand() {
        out_animation.start()
    }
    
    function shrink() {
        in_animation.start()
    }
    
    DssMultiDeleteButton {
        id: button
        opacity: 0
        
        onClicked: {
            root.action()
        }

        anchors.left: parent.left
        anchors.leftMargin: root.leftPadding
        anchors.verticalCenter: parent.verticalCenter
    }

    Loader { id: loader }

    SequentialAnimation {
        id: out_animation
        SmoothedAnimation { target: loader; property: "x"; from: 0; to: root.moveDelta; duration: 300}
        SmoothedAnimation { target: button; property: "opacity"; from: 0; to: 1; duration: 3000}
        onStopped: { root.expanded() }
    }

    SequentialAnimation {
        id: in_animation
        SmoothedAnimation { target: button; property: "opacity"; from: 1; to: 0; duration: 300}
        SmoothedAnimation { target: loader; property: "x"; from: root.moveDelta; to: 0; duration: 300}
        onStopped: { root.shrinked() }        
    }
}