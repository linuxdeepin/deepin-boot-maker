import QtQuick 2.1

Canvas {
    id: canvas
    width: 20
    height: 300
    smooth: true
    antialiasing: true

    property int pointerPos: Math.floor(height / 2)
    property int pointerHeight: 10
    property int pointerWidth: 20

    onWidthChanged: requestPaint()
    onHeightChanged: requestPaint()
    onPointerPosChanged: requestPaint()

    onPaint: {
        var ctx = getContext("2d")
        ctx.lineWidth = 2
        ctx.clearRect(0, 0, canvas.width, canvas.height)

        ctx.beginPath()
        ctx.moveTo(width - 3, 0)
        ctx.lineTo(width - 3, pointerPos - pointerWidth / 2)
        var gradientUp = ctx.createLinearGradient(width - 3, 0, width - 3, pointerPos - pointerWidth / 2);
        gradientUp.addColorStop(0.0, Qt.rgba(1, 1, 1, 0.0));
        gradientUp.addColorStop(1.0, Qt.rgba(1, 1, 1, 0.2));
        ctx.strokeStyle = gradientUp
        ctx.stroke()
        ctx.closePath()
        
        ctx.beginPath()
        ctx.moveTo(width - 3, pointerPos - pointerWidth / 2)
        ctx.lineTo(width - 3 - pointerHeight, pointerPos)
        ctx.lineTo(width - 3, pointerPos + pointerWidth / 2)
        ctx.strokeStyle = Qt.rgba(1, 1, 1, 0.2)
        ctx.stroke()
        ctx.closePath()
        
        ctx.beginPath()
        ctx.moveTo(width - 3, pointerPos + pointerWidth / 2)
        ctx.lineTo(width - 3, height)
        var gradientBottom = ctx.createLinearGradient(width - 3, pointerPos + pointerWidth / 2, width - 3, height);
        gradientBottom.addColorStop(0.0, Qt.rgba(1, 1, 1, 0.2));
        gradientBottom.addColorStop(1.0, Qt.rgba(1, 1, 1, 0.0));
        ctx.strokeStyle = gradientBottom
        ctx.stroke()
        ctx.closePath()
        
        /* separator */
        
        ctx.beginPath()
        ctx.moveTo(width - 2, 0)
        ctx.lineTo(width - 2, pointerPos - pointerWidth / 2)
        gradientUp = ctx.createLinearGradient(width - 2, 0, width - 2, pointerPos - pointerWidth / 2);
        gradientUp.addColorStop(0.0, Qt.rgba(0, 0, 0, 0.0));
        gradientUp.addColorStop(1.0, Qt.rgba(0, 0, 0, 0.7));
        ctx.strokeStyle = gradientUp
        ctx.stroke()
        ctx.closePath()
        
        ctx.beginPath()
        ctx.moveTo(width - 2, pointerPos - pointerWidth / 2)
        ctx.lineTo(width - 2 - pointerHeight, pointerPos)
        ctx.lineTo(width - 2, pointerPos + pointerWidth / 2)
        ctx.strokeStyle = Qt.rgba(0, 0, 0, 0.7)
        ctx.stroke()
        ctx.closePath()
        
        ctx.beginPath()
        ctx.moveTo(width - 2, pointerPos + pointerWidth / 2)
        ctx.lineTo(width - 2, height)
        gradientBottom = ctx.createLinearGradient(width - 2, pointerPos + pointerWidth / 2, width - 2, height);
        gradientBottom.addColorStop(0.0, Qt.rgba(0, 0, 0, 0.7));
        gradientBottom.addColorStop(1.0, Qt.rgba(0, 0, 0, 0.0));
        ctx.strokeStyle = gradientBottom
        ctx.stroke()
        ctx.closePath()
    }
}