import QtQuick 2.1
import QtGraphicalEffects 1.0

Item {
    width: 20
    height: 300

    property alias pointerPos: canvas.pointerPos
    property alias pointerHeight: canvas.pointerWidth
    property alias pointerWidth: canvas.pointerHeight

    Canvas {
        id: canvas
        smooth: true
        width: parent.width
        height: parent.height
        antialiasing: false

        property int pointerPos: Math.floor(height / 2)
        property int pointerHeight: 10
        property int pointerWidth: 20

        onWidthChanged: requestPaint()
        onHeightChanged: requestPaint()
        onPointerPosChanged: requestPaint()

        onPaint: {
            var ctx = getContext("2d")
            ctx.lineWidth = 1
            ctx.clearRect(0, 0, canvas.width, canvas.height)
            var topLineLength = pointerPos - pointerWidth / 2
            var bottomLineLength = height - (pointerPos + pointerWidth / 2)

            ctx.beginPath()
            ctx.moveTo(width - 3, 0)
            ctx.lineTo(width - 3, pointerPos - pointerWidth / 2)
            var gradientUp = ctx.createLinearGradient(width - 3, 0, width - 3, pointerPos - pointerWidth / 2);
            gradientUp.addColorStop(0.0, Qt.rgba(0.5, 0.5, 0.5, 0.0));
            gradientUp.addColorStop(Math.min(1.0, 10 / topLineLength), Qt.rgba(0.5, 0.5, 0.5, 0.2));
            gradientUp.addColorStop(1.0, Qt.rgba(0.5, 0.5, 0.5, 0.2));
            ctx.strokeStyle = gradientUp
            ctx.stroke()
            ctx.closePath()

            ctx.beginPath()
            ctx.moveTo(width - 3, pointerPos - pointerWidth / 2)
            ctx.lineTo(width - 3 - pointerHeight, pointerPos)
            ctx.lineTo(width - 3, pointerPos + pointerWidth / 2)
            ctx.strokeStyle = Qt.rgba(0.5, 0.5, 0.5, 0.2)
            ctx.stroke()
            ctx.closePath()

            ctx.beginPath()
            ctx.moveTo(width - 3, pointerPos + pointerWidth / 2)
            ctx.lineTo(width - 3, height)
            var gradientBottom = ctx.createLinearGradient(width - 3, pointerPos + pointerWidth / 2, width - 3, height);
            gradientBottom.addColorStop(0.0, Qt.rgba(0.5, 0.5, 0.5, 0.2));
            gradientBottom.addColorStop(bottomLineLength > 10 ? (bottomLineLength - 10) / bottomLineLength : 0.0,
                                        Qt.rgba(0.5, 0.5, 0.5, 0.2));
            gradientBottom.addColorStop(1.0, Qt.rgba(0.5, 0.5, 0.5, 0.0));
            ctx.strokeStyle = gradientBottom
            ctx.stroke()
            ctx.closePath()
        }
    }

    DropShadow {
        anchors.fill: canvas
        horizontalOffset: 2
        verticalOffset: 0
        radius: 4.0
        spread: 0.8
        samples: 16
        color: Qt.rgba(0, 0, 0, 0.8)
        source: canvas
    }
}
