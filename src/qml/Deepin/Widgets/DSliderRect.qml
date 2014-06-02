import QtQuick 2.1
import QtQuick.Controls 1.0

Item {
    id: sliderRect
    property alias value: dslider.value

    property string leftLabel: ""
    property string rightLabel: ""
    property var rulerData: [{"label": "L", "value": 0}, {"label": "R", "value": 1}]
    property real realValue: dslider.value

    property var listModelComponent: DListModelComponent {}
    property var rulerModel: {
        var model = listModelComponent.createObject(sliderRect, {})
        if(leftLabel && rightLabel){
            model.append({"label": leftLabel, "value": 0})
            model.append({"label": rightLabel, "value": 1})
        }
        else{
            for(var i in rulerData){
                model.append({"label": rulerData[i].label, "value": rulerData[i].value})
            }
        }
        return model
    }

    height: 20 + dslider.height
    width: 180

    property int paddingRuler: 2

    DssH4 {
        id: valueTip
        x: dslider.grooveWidth * dslider.value + dslider.handleWidth/2 - width/2
        text: realValue.toFixed(2)
        visible: dslider.pressed
    }

    DSlider {
        id: dslider
        width: sliderRect.width
        anchors.verticalCenter: parent.verticalCenter
    }

    Repeater{
        model: rulerModel
        delegate: DssH4 {
            anchors.top: dslider.bottom
            anchors.topMargin: -2
            x: dslider.grooveWidth * value + dslider.handleWidth/2 - width/2
            text: label
        }
    }
}
