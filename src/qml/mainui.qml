import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.0
import Qt.labs.folderlistmodel 2.1
import QtQuick.Controls.Styles 1.1
import Deepin.Widgets 1.0
import com.deepin.usbcreator 1.0

DWindow {
    id: usbCreatorUI
    flags: Qt.FramelessWindowHint
    color: "transparent"
    width: 660 + windowFrame.shadowRadius * 2
    height: 400 + windowFrame.shadowRadius * 2

    UsbCreator {
        id: usbCreator
        objectName: "usbCreatorDeepin"
        property string isoImagePath: ""
        property string usbDriver: ""
        property string uiFont: "WenQuanYi Micro Hei"
        property int lableMaxWidth: 380
    }

    DDragableArea {
        window: usbCreatorUI
        width: 660
        height: 30
    }

    Timer {
        id: processTimer
        interval: 2000
        running: false
        repeat: true
        onTriggered: {
            processRate.text = "%1%".arg(usbCreator.processRate())
            console << processRate.text
            if (usbCreator.isFinish()) {
                processTimer.stop()
                processNext.clicked()
            }
        }
    }

    Timer {
        id: usbRefreshTimer
        interval: 3000
        running: true
        repeat: true
        onTriggered: {
            usbCreatorUI.refreshUsbDriver()
        }
    }

    function refreshUsbDriver() {
        var oldCurText = usbDriver.currentText
        var usblist = usbCreator.listUsbDrives()
        var index = 0
        usbDriver.model = usblist
        for (var i = 0; i < usblist.length; i++) {
            if (usbDriver.model[i] === oldCurText) {
                index = i
            }
        }
        usbDriver.currentIndex = index
        if (usbDriver.model.length > 0) {
            usbIcon.source = "qrc:/image/usb-active.png"
        }
    }

    DWindowFrame {
        id: windowFrame
        anchors.fill: parent

        Row {
            Rectangle {
                color: "transparent"
                width: 230
                height: 400
                Column {
                    anchors.centerIn: parent
                    width: 64
                    height: 64 * 3
                    Image {
                        id: isoIcon
                        source: "qrc:/image/iso-inactive.png"
                    }
                    AnimatedImage {
                        id: process
                        source: "qrc:/image/process-inactive.png"
                    }
                    Rectangle {
                        width: 64
                        height: 64
                        color: "transparent"
                        Image {
                            id: usbIcon
                            source: "qrc:/image/usb-inactive.png"
                        }
                        DLabel {
                            id: processRate
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: ""
                            font.pixelSize: 10
                        }
                    }
                }
            }
            Rectangle {
                color: "transparent"
                width: 430
                height: 400
                Column {
                    Rectangle {
                        color: "transparent"
                        width: 430
                        height: 100
                        Row {
                            Rectangle {
                                color: "transparent"
                                width: 230
                                height: 100
                                Image {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    id: logo
                                    source: "qrc:/image/logo.png"
                                }
                            }
                            Rectangle {
                                color: "transparent"
                                width: 195
                                height: 100
                                DImageButton {
                                    id: btClose
                                    anchors.right: parent.right
                                    normal_image: "qrc:/image/window_close_normal.png"
                                    hover_image: "qrc:/image/window_close_hover.png"
                                    press_image: "qrc:/image/window_close_press.png"

                                    onClicked: {
                                        usbCreatorUI.close()
                                    }
                                }
                            }
                        }
                    }
                    Rectangle {
                        color: "transparent"
                        width: 430
                        height: 70
                        Column {
                            DLabel {
                                id: descriptionsBye
                                font.pixelSize: 12
                                width: usbCreator.lableMaxWidth
                                font.family: usbCreator.uiFont
                                wrapMode: Text.Wrap
                                text: qsTr("<font color='#ffffff'>Say goodbye to cumbersome, easy to use !</font></br>")
                            }
                            DLabel {
                                id: descriptions
                                font.pixelSize: 11
                                width: usbCreator.lableMaxWidth
                                font.family: usbCreator.uiFont
                                wrapMode: Text.Wrap
                                //text: "欢迎使用深度启动盘软件，通过简单设置即可快速制作Deepin系统启动盘，并且支持BIOS和UEFI双启动。"
                                text: qsTr("<br><font color='#a7a7a7'>Does not require any technical basis, breakthrough Technical barriers and heavy driver, rookie can also get along well with Linux, support <font color='#ebab4c'>UEFI.</font></font></br>")
                            }
                        }
                    }
                    Rectangle {
                        id: steps
                        color: "transparent"
                        width: 430
                        height: 230
                        Column {
                            id: firstStep
                            Rectangle {
                                id: rcIso
                                color: "transparent"
                                width: 430
                                height: 75
                                Column {
                                    DLabel {
                                        id: selectIsoHits
                                        font.pixelSize: 12
                                        width: usbCreator.lableMaxWidth
                                        font.family: usbCreator.uiFont
                                        wrapMode: Text.Wrap
                                        text: qsTr("<br><font color='#ffffff'>Select the ISO image file:</font></br>")
                                    }
                                    Rectangle {
                                        color: "transparent"
                                        width: 430
                                        height: 6
                                    }
                                    DFileChooseInput {
                                        id: isoPath
                                        width: 210
                                        FileDialog {
                                            id: isoFileChoose
                                            visible: false
                                            nameFilters: ["ISO files (*.iso)"]
                                            onAccepted: {
                                                //check
                                                usbCreator.isoImagePath = usbCreator.url2LocalFile(
                                                            isoFileChoose.fileUrl)
                                                if (usbCreator.isISOImage(
                                                            usbCreator.isoImagePath)) {
                                                    usbCreator.isoImagePath
                                                            = usbCreator.isoImagePath
                                                    isoPath.textInput.text = usbCreator.isoImagePath
                                                    isoIcon.source = "qrc:/image/iso-active.png"
                                                } else {
                                                    usbCreator.isoImagePath = ""
                                                    isoPath.textInput.text = ""
                                                }
                                                usbCreatorUI.refreshUsbDriver()
                                            }
                                        }

                                        onFileChooseClicked: {
                                            isoFileChoose.open()
                                        }
                                    }
                                }
                            }
                            Rectangle {
                                id: rcUsb
                                color: "transparent"
                                width: 430
                                height: 55
                                Column {
                                    anchors.verticalCenter: parent.verticalCenter
                                    DLabel {
                                        id: selectUsbHits
                                        font.pixelSize: 12
                                        width: usbCreator.lableMaxWidth
                                        font.family: usbCreator.uiFont
                                        wrapMode: Text.Wrap
                                        text: qsTr("<br><font color='#ffffff'>Select the USB disk file:</font></br>")
                                    }
                                    Rectangle {
                                        color: "transparent"
                                        width: 430
                                        height: 5
                                    }
                                    ComboBox {
                                        id: usbDriver
                                        width: 210
                                        height: isoPath.height
                                        model: usbCreator.listUsbDrives()

                                        style: ComboBoxStyle {
                                            QtObject {
                                                id: buttonImage
                                                property string status: usbDriver.pressed ? "press" : "normal"
                                                property string header: "qrc:/image/button_left_%1.png".arg(
                                                                            status)
                                                property string middle: "qrc:/image/button_center_%1.png".arg(
                                                                            status)
                                                property string tail: "qrc:/image/button_right_%1.png".arg(
                                                                          status)
                                            }
                                            background: Rectangle {
                                                id: rectCategory
                                                width: usbDriver.width
                                                height: usbDriver.height
                                                color: Qt.rgba(25 / 255,
                                                               26 / 255,
                                                               28 / 255, 1)

                                                Row {
                                                    id: background
                                                    height: buttonHeader.height
                                                    width: parent.width

                                                    Image {
                                                        id: buttonHeader
                                                        source: buttonImage.header
                                                    }

                                                    Image {
                                                        id: buttonMiddle
                                                        source: buttonImage.middle
                                                        width: parent.width - buttonHeader.width
                                                               - buttonTail.width
                                                    }

                                                    Image {
                                                        id: buttonTail
                                                        source: buttonImage.tail
                                                    }
                                                }

                                                Image {
                                                    id: downArrow
                                                    anchors.right: parent.right
                                                    anchors.rightMargin: 5
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    source: usbDriver.activeFocusOnPress ? "qrc:/image/arrow_down_hover.png" : "qrc:/image/arrow_down_normal.png"
                                                }
                                            }

                                            label: DLabel {
                                                text: control.currentText
                                            }
                                        }
                                    }

                                    Rectangle {
                                        color: "transparent"
                                        width: 430
                                        height: 5
                                    }

                                    DCheckBox {
                                        id: bisoMode
                                        text: qsTr("<font color='#ffffff'>BIOS Mode. Check if the usb stick can not work</font>")
                                    }
                                }
                            }
                            Rectangle {
                                id: rcStart
                                color: "transparent"
                                width: 230
                                height: 100
                                DTransparentButton {
                                    id: startBt
                                    text: qsTr("Start")
                                    width: 100
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    onClicked: {
                                        //Debug Code
                                        //secondStep.visible = true
                                        //  firstStep.visible = false
                                        //processNext.visible = true
                                        var result = usbCreator.start(
                                                    isoPath.text,
                                                    usbDriver.currentText,
                                                    bisoMode.checked)
                                        if (0 === result) {
                                            //make the iso/usb selector invisible
                                            firstStep.visible = false
                                            secondStep.visible = true
                                            process.source = "qrc:/image/process-active.gif"
                                            process.playing = true
                                            processTimer.start()
                                            btClose.visible = false
                                        }
                                        console.log(isoPath.text + usbDriver.currentText)
                                    }
                                }
                            }
                        }
                        Column {
                            id: secondStep
                            visible: false
                            Rectangle {
                                id: warningHits
                                width: 460
                                height: 130
                                color: "transparent"
                                DLabel {
                                    id: warningLabel
                                    anchors.verticalCenter: parent.verticalCenter
                                    text: qsTr("<font color='#ffffff'><br>Writing USB device, <font color='#ebab4c'>DO NOT</font> pull out the USB equipment the </br><br>power off !</br></font>")
                                    font.pixelSize: 12
                                    //wrapMode: TextEdit.WordWrap
                                }
                            }
                            Rectangle {
                                width: 230
                                height: 100
                                color: "transparent"
                                DTransparentButton {
                                    id: processNext
                                    text: "Next"
                                    width: 100
                                    visible: false
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    onClicked: {
                                        //make the iso/usb selector invisible
                                        secondStep.visible = false
                                        thirdStep.visible = true
                                        btClose.visible = true
                                    }
                                }
                            }
                        }
                        Column {
                            id: thirdStep
                            visible: false
                            Rectangle {
                                id: finishHits
                                width: 460
                                height: 130
                                color: "transparent"
                                DLabel {
                                    id: finishLabel
                                    anchors.verticalCenter: parent.verticalCenter
                                    text: qsTr("<br><font color='#057aff'>Congratulations!</font></br><br><font color='#ffffff'>The deepin USB installation disk has produced success.</font></br>")
                                    font.pixelSize: 12
                                    wrapMode: TextEdit.WordWrap
                                }
                            }
                            Rectangle {
                                width: 230
                                height: 100
                                color: "transparent"
                                Row {
                                    DTransparentButton {
                                        text: qsTr("Complete")
                                        width: 100
                                        anchors.verticalCenter: parent.verticalCenter
                                        //anchors.horizontalCenter: parent.horizontalCenter
                                        onClicked: {
                                            usbCreatorUI.close()
                                        }
                                    }
                                    DTransparentButton {
                                        text: qsTr("Restart")
                                        width: 100
                                        anchors.verticalCenter: parent.verticalCenter
                                        //anchors.horizontalCenter: parent.horizontalCenter
                                        onClicked: {
                                            usbCreator.exitRestart()
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
