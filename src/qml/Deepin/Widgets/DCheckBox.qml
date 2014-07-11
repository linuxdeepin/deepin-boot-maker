import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

CheckBox {
    property int fontSize: 12
    
    text: "hello"
    style: CheckBoxStyle {
        background: Item {}
        indicator: Image {
            source: control.checked ? control.hovered ? "images/checkbox_checked_hover.png" : "images/checkbox_checked.png" 
                                    : control.hovered ? "images/checkbox_unchecked_hover.png" : "images/checkbox_unchecked.png"
        }
        label: Text {
            color: Qt.rgba(1, 1, 1, 0.5)
            text: control.text
            font.pixelSize: control.fontSize
        }        
    }
}