import QtQuick 2.1
import QtGraphicalEffects 1.0

Item {
    id: window
    
    property int frameRadius: 3
    property int shadowRadius: 10
    property alias frame: frame
    
    default property alias content: container.children
    
    Rectangle {
        id: frame
        anchors.centerIn: parent
        color: "#232323"
        radius: frameRadius
        border.width: 1
        border.color: Qt.rgba(1, 1, 1, 0.3)
        width: window.width - (shadowRadius + frameRadius) * 2
        height: window.height - (shadowRadius + frameRadius) * 2

		Item {
            id: container
			anchors.fill: parent
		}
    }
    
    RectangularGlow {
        id: shadow
        z: -1
        anchors.fill: frame
        glowRadius: shadowRadius
        spread: 0.2
        color: Qt.rgba(0, 0, 0, 0.3)
        cornerRadius: frame.radius + shadowRadius
        visible: true
    }
}
