import QtQuick 2.1
import QtQuick.Window 2.1
import QtGraphicalEffects 1.0
import Deepin.Widgets 1.0

DWindow {
	id: root
	width: 300
	height: 300
	color: "transparent"
	flags: Qt.FramelessWindowHint | Qt.SubWindow
    
    property int titleContentPadding: 5
	property int windowGlowRadius: 8
    property alias title: titlebar_title.text
    property alias content: loader.sourceComponent

	DConstants { id: dconstants }

	RectangularGlow {
	    id: shadow
	    anchors.fill: rect
	    glowRadius: root.windowGlowRadius - 5
	    spread: 0
	    color: Qt.rgba(0, 0, 0, 1)
	    cornerRadius: 10
	    visible: true
	}

	Rectangle {
		id: rect
		clip: true
		radius: 4
		color: dconstants.contentBgColor
		width: root.width - root.windowGlowRadius * 2
		height: root.height - root.windowGlowRadius * 2
        border.width: 1
        border.color: "black"
		anchors.centerIn: parent
        
        Rectangle {
            width: parent.width - 2
            height: parent.height - 2
            anchors.centerIn: parent
            color: "transparent"
            radius: 3
            border.width: 1
            border.color: Qt.rgba(1, 1, 1, 0.1)
        }

		DDragableArea {
			id: titlebar
            z: loader.z + 1
			width: rect.width
			height: close_button.height
            window: root
            
            Text {
                id: titlebar_title
                color: "white"
                font.pixelSize: 10
                width: Math.min(parent.width - 40, implicitWidth)
                anchors.verticalCenter: close_button.verticalCenter
                anchors.horizontalCenter: titlebar.horizontalCenter
            }

			DImageButton {
			    id: close_button
			    normal_image: "images/window_close_normal.png"
                hover_image: "images/window_close_hover.png"
                press_image: "images/window_close_press.png"
			    anchors.top: parent.top
			    anchors.right: parent.right

			    onClicked: { root.close() }
			}
		}
        
        Loader {
            id: loader
            sourceComponent: root.content
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: titlebar.bottom
            anchors.bottom: parent.bottom
            anchors.topMargin: root.titleContentPadding
            anchors.bottomMargin: 10
        }
	}
}