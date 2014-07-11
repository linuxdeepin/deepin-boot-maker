import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

RadioButton {
    property int fontSize: 12
    
    style: RadioButtonStyle {
        background: Item {}
        indicator: Image {
            source: control.checked ? control.hovered ? "images/radio_selected_hover.png" : "images/radio_selected.png" 
                                    : control.hovered ? "images/radio_unselected_hover.png" : "images/radio_unselected.png"
        }
        label: Text {
            color: Qt.rgba(1, 1, 1, 0.5)
            text: control.text
            font.pixelSize: control.fontSize
        }
    }
}