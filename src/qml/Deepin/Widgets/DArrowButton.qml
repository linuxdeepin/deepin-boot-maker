import QtQuick 2.1

DImageButton {
    property string direction: "right"

    normal_image: "images/arrow_"+ direction +"_normal.png"
    hover_image: "images/arrow_"+ direction +"_hover.png"
    press_image: "images/arrow_"+ direction + "_press.png"
}
