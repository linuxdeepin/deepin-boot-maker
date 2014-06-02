/*****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtQuick.Dialogs module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
*****************************************************************************/

import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.0
import QtQuick.Window 2.1
import "qml"

AbstractColorDialog {
    id: root
    property bool __valueSet: true // guard to prevent binding loops
    function __setControlsFromColor() {
        __valueSet = false
        hueSlider.value = root.currentHue
        saturationSlider.value = root.currentSaturation
        lightnessSlider.value = root.currentLightness
        alphaSlider.value = root.currentAlpha
        crosshairs.x = root.currentLightness * paletteMap.width
        crosshairs.y = (1.0 - root.currentSaturation) * paletteMap.height
        __valueSet = true
    }
    onCurrentColorChanged: __setControlsFromColor()

    Rectangle {
        id: content
        property int maxSize: 0.9 * Math.min(Screen.desktopAvailableWidth, Screen.desktopAvailableHeight)
        implicitHeight: Math.min(maxSize, Screen.pixelDensity * (usePaletteMap ? 100 : 50))
        implicitWidth: usePaletteMap ? implicitHeight - bottomMinHeight : implicitHeight * 1.5
        color: palette.window
        focus: root.visible
        property real bottomMinHeight: sliders.height + buttonRow.height + outerSpacing * 3
        property real spacing: 8
        property real outerSpacing: 12
        property bool usePaletteMap: true

        Keys.onPressed: {
            event.accepted = true
            switch (event.key) {
            case Qt.Key_Return:
            case Qt.Key_Select:
                accept()
                break
            case Qt.Key_Escape:
            case Qt.Key_Back:
                reject()
                break
            case Qt.Key_C:
                if (event.modifiers & Qt.ControlModifier)
                    colorField.copyAll()
                break
            case Qt.Key_V:
                if (event.modifiers & Qt.ControlModifier) {
                    colorField.paste()
                    root.currentColor = colorField.text
                }
                break
            default:
                // do nothing
                event.accepted = false
                break
            }
        }

        // set the preferred width based on height, to avoid "letterboxing" the paletteMap
        onHeightChanged: implicitHeight = Math.max((usePaletteMap ? 480 : bottomMinHeight), height)

        SystemPalette { id: palette }

        Item {
            id: paletteFrame
            visible: content.usePaletteMap
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                margins: content.outerSpacing
            }
            height: Math.min(content.height - content.bottomMinHeight, content.width - content.outerSpacing * 2)

            Image {
                id: paletteMap
                x: (parent.width - width) / 2
                width: height
                onWidthChanged: root.__setControlsFromColor()
                height: parent.height
                source: "images/checkers.png"
                fillMode: Image.Tile

                // note we smoothscale the shader from a smaller version to improve performance
                ShaderEffect {
                    id: map
                    width: 64
                    height: 64
                    opacity: alphaSlider.value
                    scale: paletteMap.width / width;
                    layer.enabled: true
                    layer.smooth: true
                    anchors.centerIn: parent
                    property real hue: hueSlider.value

                    fragmentShader: "
                    varying mediump vec2 qt_TexCoord0;
                    uniform highp float qt_Opacity;
                    uniform highp float hue;

                    highp float hueToIntensity(highp float v1, highp float v2, highp float h) {
                        h = fract(h);
                        if (h < 1.0 / 6.0)
                            return v1 + (v2 - v1) * 6.0 * h;
                        else if (h < 1.0 / 2.0)
                            return v2;
                        else if (h < 2.0 / 3.0)
                            return v1 + (v2 - v1) * 6.0 * (2.0 / 3.0 - h);

                        return v1;
                    }

                    highp vec3 HSLtoRGB(highp vec3 color) {
                        highp float h = color.x;
                        highp float l = color.z;
                        highp float s = color.y;

                        if (s < 1.0 / 256.0)
                            return vec3(l, l, l);

                        highp float v1;
                        highp float v2;
                        if (l < 0.5)
                            v2 = l * (1.0 + s);
                        else
                            v2 = (l + s) - (s * l);

                        v1 = 2.0 * l - v2;

                        highp float d = 1.0 / 3.0;
                        highp float r = hueToIntensity(v1, v2, h + d);
                        highp float g = hueToIntensity(v1, v2, h);
                        highp float b = hueToIntensity(v1, v2, h - d);
                        return vec3(r, g, b);
                    }

                    void main() {
                        lowp vec4 c = vec4(1.0);
                        c.rgb = HSLtoRGB(vec3(hue, 1.0 - qt_TexCoord0.t, qt_TexCoord0.s));
                        gl_FragColor = c * qt_Opacity;
                    }
                    "
                }

                MouseArea {
                    id: mapMouseArea
                    anchors.fill: parent
                    onPositionChanged: {
                        if (pressed && containsMouse) {
                            var xx = Math.max(0, Math.min(mouse.x, parent.width))
                            var yy = Math.max(0, Math.min(mouse.y, parent.height))
                            saturationSlider.value = 1.0 - yy / parent.height
                            lightnessSlider.value = xx / parent.width
                            // TODO if we constrain the movement here, can avoid the containsMouse test
                            crosshairs.x = mouse.x - crosshairs.radius
                            crosshairs.y = mouse.y - crosshairs.radius
                        }
                    }
                    onPressed: positionChanged(mouse)
                }

                Image {
                    id: crosshairs
                    property int radius: width / 2 // truncated to int
                    source: "images/crosshairs.png"
                }

                BorderImage {
                    anchors.fill: parent
                    anchors.margins: -1
                    anchors.leftMargin: -2
                    source: "images/sunken_frame.png"
                    border.left: 8
                    border.right: 8
                    border.top: 8
                    border.bottom: 8
                }
            }
        }

        Column {
            id: sliders
            anchors {
                top: paletteFrame.bottom
                left: parent.left
                right: parent.right
                margins: content.outerSpacing
            }
            spacing: content.spacing

            ColorSlider {
                id: hueSlider
                value: 0.5
                onValueChanged: if (__valueSet) root.currentColor = Qt.hsla(hueSlider.value, saturationSlider.value, lightnessSlider.value, alphaSlider.value)
                text: qsTr("Hue")
                trackDelegate: Rectangle {
                    rotation: -90
                    transformOrigin: Item.TopLeft
                    gradient: Gradient {
                        GradientStop {position: 0.000; color: Qt.rgba(1, 0, 0, 1)}
                        GradientStop {position: 0.167; color: Qt.rgba(1, 1, 0, 1)}
                        GradientStop {position: 0.333; color: Qt.rgba(0, 1, 0, 1)}
                        GradientStop {position: 0.500; color: Qt.rgba(0, 1, 1, 1)}
                        GradientStop {position: 0.667; color: Qt.rgba(0, 0, 1, 1)}
                        GradientStop {position: 0.833; color: Qt.rgba(1, 0, 1, 1)}
                        GradientStop {position: 1.000; color: Qt.rgba(1, 0, 0, 1)}
                    }
                }
            }

            ColorSlider {
                id: saturationSlider
                visible: !content.usePaletteMap
                value: 0.5
                onValueChanged: if (__valueSet) root.currentColor = Qt.hsla(hueSlider.value, saturationSlider.value, lightnessSlider.value, alphaSlider.value)
                text: qsTr("Saturation")
                trackDelegate: Rectangle {
                    rotation: -90
                    transformOrigin: Item.TopLeft
                    gradient: Gradient {
                        GradientStop { position: 0; color: Qt.hsla(hueSlider.value, 0.0, lightnessSlider.value, 1.0) }
                        GradientStop { position: 1; color: Qt.hsla(hueSlider.value, 1.0, lightnessSlider.value, 1.0) }
                    }
                }
            }

            ColorSlider {
                id: lightnessSlider
                visible: !content.usePaletteMap
                value: 0.5
                onValueChanged: if (__valueSet) root.currentColor = Qt.hsla(hueSlider.value, saturationSlider.value, lightnessSlider.value, alphaSlider.value)
                text: qsTr("Luminosity")
                trackDelegate: Rectangle {
                    rotation: -90
                    transformOrigin: Item.TopLeft
                    gradient: Gradient {
                        GradientStop { position: 0; color: "black" }
                        GradientStop { position: 0.5; color: Qt.hsla(hueSlider.value, saturationSlider.value, 0.5, 1.0) }
                        GradientStop { position: 1; color: "white" }
                    }
                }
            }

            ColorSlider {
                id: alphaSlider
                minimum: 0.0
                maximum: 1.0
                value: 1.0
                onValueChanged: if (__valueSet) root.currentColor = Qt.hsla(hueSlider.value, saturationSlider.value, lightnessSlider.value, alphaSlider.value)
                text: qsTr("Alpha")
                visible: root.showAlphaChannel
                trackDelegate: Item {
                    rotation: -90
                    transformOrigin: Item.TopLeft
                    Image {
                        anchors {fill: parent}
                        source: "images/checkers.png"
                        fillMode: Image.TileVertically
                    }
                    Rectangle {
                        anchors.fill: parent
                        gradient: Gradient {
                            GradientStop { position: 0; color: "transparent" }
                            GradientStop { position: 1; color: Qt.hsla(hueSlider.value,
                                                                       saturationSlider.value,
                                                                       lightnessSlider.value, 1.0) }
                        }                    }
                }
            }
        }

        Item {
            id: buttonRow
            height: Math.max(buttonsOnly.height, copyIcon.height)
            width: parent.width
            anchors {
                left: parent.left
                right: parent.right
                bottom: content.bottom
                margins: content.outerSpacing
            }
            Row {
                spacing: content.spacing
                height: parent.height
                TextField {
                    id: colorField
                    text: root.currentColor.toString()
                    anchors.verticalCenter: parent.verticalCenter
                    onAccepted:  root.currentColor = text
                    Component.onCompleted: width = implicitWidth + 10
                }
                Image {
                    id: copyIcon
                    anchors.verticalCenter: parent.verticalCenter
                    source: "images/copy.png"
                    MouseArea {
                        anchors.fill: parent
                        onClicked: colorField.copyAll()
                    }
                }
            }
            Row {
                id: buttonsOnly
                spacing: content.spacing
                anchors.right: parent.right
                Button {
                    id: cancelButton
                    text: "Cancel"
                    onClicked: root.reject()
                }
                Button {
                    id: okButton
                    text: "OK"
                    onClicked: root.accept()
                }
            }
        }
    }
}
