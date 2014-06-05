import QtQuick 2.1
import QtGraphicalEffects 1.0

Item {
	id: container
	property variant target
	width: target.width; height: target.height
	property alias radius: mask.radius
	
	Rectangle {
		anchors.fill: container
		id: mask
		radius: container.radius
		antialiasing: true
		smooth: true
	}
	
    OpacityMask {
		anchors.fill: parent
        source: ShaderEffectSource { sourceItem: target; hideSource: true }
        maskSource: ShaderEffectSource{ sourceItem: mask; hideSource: true }
    }
}
	
