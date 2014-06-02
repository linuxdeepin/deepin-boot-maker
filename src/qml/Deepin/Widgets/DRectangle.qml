import QtQuick 2.1
import QtGraphicalEffects 1.0

Rectangle {
    id: rect
    default property alias content: container.children    
    
    width: 200
    height: 200
    color: Qt.rgba(0, 0, 0, 0)

    property int borderMargin: 5    
    property bool withBlur: true
    
    property color fillColor: Qt.rgba(1, 1, 1, 0.9)
    property color blurColor: Qt.rgba(0, 0, 0, 0.6)
    property color borderColor: Qt.rgba(0, 0, 0, 0.15)

    property int blurRadius: 16
    property int blurWidth: 10
    property int borderWidth: 2
    property int rectRadius: 4
    property rect contentRect
    property int sideWidth: blurWidth + borderMargin

    Canvas {
        id: canvas
        width: rect.width
        height: rect.height
        
        onWidthChanged: requestPaint()        
        onHeightChanged: requestPaint()
        
        onPaint: {
            var ctx = getContext("2d")

            ctx.save()
            ctx.clearRect(0, 0, canvas.width, canvas.height)

            ctx.beginPath();

            var x = blurWidth
            var y = blurWidth
            var w = rect.width - 2 * blurWidth
            var h = rect.height - 2 * blurWidth
            rect.contentRect = Qt.rect(x, y, w, h)
            ctx.roundedRect(x, y, w, h, rectRadius, rectRadius)
            ctx.closePath()

            ctx.lineWidth = borderWidth
            ctx.strokeStyle = borderColor
            ctx.stroke()

            ctx.fillStyle = fillColor
            ctx.fill()
            ctx.restore()
        }
    }

    DropShadow {
        anchors.fill: canvas
        horizontalOffset: 0
        verticalOffset: 3
        radius: blurWidth
        samples: 16
        color: rect.blurColor
        source: canvas
    }    
    
    Item {
        id: container
        x: rect.contentRect.x + borderMargin
        y: rect.contentRect.y + borderMargin
        width: rect.contentRect.width - borderMargin * 2
        height: rect.contentRect.height - borderMargin * 2
    }
}
