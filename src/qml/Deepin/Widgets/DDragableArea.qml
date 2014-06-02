import QtQuick 2.1

MouseArea {
	property var window
    
	property int dragStartX
	property int dragStartY
    property int windowLastX
    property int windowLastY

	onPressed: { 
        var pos = window.getCursorPos()
        
        windowLastX = window.x
        windowLastY = window.y
        dragStartX = pos.x
        dragStartY = pos.y 
    }
	onPositionChanged: { 
        if (pressed) {
            var pos = window.getCursorPos()
            window.setX(windowLastX + pos.x - dragStartX)
            window.setY(windowLastY + pos.y - dragStartY)
            windowLastX = window.x
            windowLastY = window.y
            dragStartX = pos.x
            dragStartY = pos.y
        }
	}
}