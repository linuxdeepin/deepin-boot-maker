import QtQuick 2.1
import QtQuick.Window 2.1
import QtGraphicalEffects 1.0

Window {
	id: win
	flags: Qt.Tool | Qt.FramelessWindowHint
    width: 400
    height: 300
    color: "transparent"

    property var parentWindow: null

    function isInRect(pos, rect){
        if(pos.x > rect.x && pos.x < rect.x + rect.width && pos.y > rect.y && pos.y < rect.y + rect.height){
            return true
        }
        else{
            return false
        }
    }

    Connections{
        target: parentWindow
        onWindowFocusChanged: {
            if(!window && win.visible){
                win.visible = false
            }
        }

        onMousePressed: {
            var pos = parentWindow.getCursorPos()
            if(!isInRect(pos, win)){
                win.visible = false
            }
        }
    }
}

