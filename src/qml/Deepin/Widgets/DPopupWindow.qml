import QtQuick 2.1
import QtQuick.Window 2.1
import QtGraphicalEffects 1.0

Window {
	id: win
	flags: Qt.Tool|Qt.FramelessWindowHint
	width: 300; height: 400		
	visible: false
	color: "transparent"
	x: 300; y: 300
	property real rectWidth
	property real rectHeight
    property color blurColor: Qt.rgba(0, 0, 0, 0.3)
    property color borderColor: Qt.rgba(255, 255, 255, 0.33)
	property color blackBorderColor: Qt.rgba(30/255.0, 30/255.0, 30/255.0, 0.6)
	property real blurWidth: 5
	property real rectRadius: 3
	property real sideWidth: blurWidth + rectRadius
	property rect vaildRect: Qt.rect(sideWidth, sideWidth, width - sideWidth * 2, height - sideWidth * 2)
	
    Rectangle {
        id: rect
		x: 0; y: 0
		width: parent.width; height: parent.height
        radius: rectRadius
		anchors.centerIn: parent		
		antialiasing: true
		color: "transparent"
		smooth: true
		
		Rectangle {
			id: backgound
			
			color: "#fff"
			anchors.fill: parent
			antialiasing: true
			smooth: true
			
			Rectangle {
				border { width: 1; color: blackBorderColor }
				anchors.fill: parent
				color: "transparent"
				radius: rectRadius
			}
			
			Rectangle {
				anchors.margins: 1
				border { width: 1; color: borderColor }
				anchors.fill: parent
				color: "transparent"
				radius: rectRadius
			}
		}
		
		DRoundItem {
			target: backgound
			radius: rectRadius
		}
    }
}

