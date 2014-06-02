import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0

Slider {
    id: slider
    width: 196
    property int grooveWidth: width - handleWidth + 2
    property int grooveHeight: 8
    property int handleWidth: 18

    property int _buttonWidth: 0

    style: SliderStyle {
        groove: Rectangle {
            implicitWidth: grooveWidth + handleWidth - 2
            implicitHeight: grooveHeight * 2
            clip: true
            color: Qt.rgba(1, 1, 1, 0)

            Rectangle{
                implicitWidth: grooveWidth
                implicitHeight: grooveHeight
                clip: true
                radius: implicitHeight
                anchors.centerIn: parent
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "black" }
                    GradientStop { position: 1.0; color: "#303132" }
                }

                Rectangle {
                    implicitWidth: parent.width - 2
                    implicitHeight: grooveHeight - 2
                    anchors.centerIn: parent
                    radius: implicitHeight
                    color: Qt.rgba(15/255, 15/255, 15/255, 1.0)

                    Rectangle {
                        width: parent.width * slider.value
                        height: parent.height
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        radius: height
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: Qt.rgba(0, 104/255, 170/255, 1.0) }
                            GradientStop { position: 1.0; color: Qt.rgba(91/255, 164/255, 211/255, 1.0) }
                        }
                    }
                }
            }


        }
        handle: Image {
            source: "images/slider_handle.svg"
            Component.onCompleted: {
                _buttonWidth = width
            }
        }
    }

}
