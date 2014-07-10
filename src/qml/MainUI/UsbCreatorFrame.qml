import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.0
import Qt.labs.folderlistmodel 2.1
import QtQuick.Controls.Styles 1.1
import Deepin.Widgets 1.0
import com.deepin.bootmaker 1.0

DWindowFrame {
    id: windowFrame
    anchors.fill: parent

    Rectangle {
        BootMaker {
            id: usbCreator
            objectName: "usbCreatorDeepin"
            property string isoImagePath: ""
            property string usbDriver: ""
            property int lableMaxWidth: 380

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
                usbCreator.refreshUsbDriver()
            }
        }
    }
    Row {
        Rectangle {
            color: "transparent"
            width: 230
            height: 420
            Column {
                anchors.centerIn: parent
                width: 64
                height: 68 * 3
                Image {
                    id: isoIcon
                    source: "qrc:/image/iso-inactive.png"
                }
                NumberAnimation {
                    id: isoAnimation
                    running: false
                    loops: Animation.Infinite
                    target: isoIcon
                    from: 0
                    to: 360
                    property: "rotation"
                    duration: 2000
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
                    Rectangle {
                        width: 50
                        height: 64
                        color: "transparent"
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
        }
        Rectangle {
            color: "transparent"
            width: 430
            height: 420
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
                            DLabel {
                                x: parent.x + 178
                                y: parent.y + 67
                                font.pixelSize: 10
                                text: "0.98"
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
                            font.pixelSize: 14
                            width: usbCreator.lableMaxWidth
                            wrapMode: Text.Wrap
                            text: qsTr("<font color='#ffffff'>Easy to use without redundancy</font></br>")
                        }
                        DLabel {
                            id: descriptions
                            font.pixelSize: 11
                            width: usbCreator.lableMaxWidth
                            wrapMode: Text.Wrap
                            text: qsTr("<br><font color='#a7a7a7'>Welcome to use Deepin Boot Maker software and you can quickly create Deepin OS Startup Disk through a simple setting, which supports dual BIOS and <font color='#ebab4c'>UEFI</font> start.</font></br>")
                        }
                    }
                }
                Rectangle {
                    id: steps
                    color: "transparent"
                    width: 430
                    height: 250
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
                                    wrapMode: Text.Wrap
                                    text: qsTr("<br><font color='#ffffff'>Select the ISO File:</font></br>")
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
                                        selectMultiple: false
                                        folder: usbCreator.homeDir()
					//nameFilters: ["ISO (*.iso);;"]
                                        onAccepted: {
                                            usbCreator.isoImagePath = usbCreator.url2LocalFile(
                                                        isoFileChoose.fileUrl)
                                            if (usbCreator.isISOImage(
                                                        usbCreator.isoImagePath)) {
                                                usbCreator.isoImagePath = usbCreator.isoImagePath
                                                isoPath.textInput.text = usbCreator.isoImagePath
                                                isoIcon.source = "qrc:/image/iso-active.png"
                                            } else {
                                                usbCreator.isoImagePath = ""
                                                isoPath.textInput.text = ""
                                            }
                                            usbCreator.refreshUsbDriver()
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
                            height: 75
                            Column {
                                anchors.verticalCenter: parent.verticalCenter
                                DLabel {
                                    id: selectUsbHits
                                    font.pixelSize: 12
                                    width: usbCreator.lableMaxWidth
                                    wrapMode: Text.Wrap
                                    text: qsTr("<br><font color='#ffffff'>Select the USB Flash Drive:</font></br>")
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
                                            color: Qt.rgba(25 / 255, 26 / 255,
                                                           28 / 255, 0)

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
                                            font.pixelSize: 11
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.horizontalCenter: parent.horizontalCenter
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
                                    text: qsTr("<font color='#ffffff'>Support BIOS. Unselect here. </font>")
                                }
                                Rectangle {
                                    color: "transparent"
                                    width: 430
                                    height: 5
                                }
                                DCheckBox {
                                    id: formatDisk
                                    text: qsTr("<font color='#ffffff'>format usb disk.</font>")

                                    MessageDialog{
                                        id: messageDialog
                                        icon: StandardIcon.Warning
                                        standardButtons: StandardButton.Ok | StandardButton.Cancel
                                        title: qsTr("Format Disk");
                                        text: qsTr("Format Disk?")
                                        onAccepted: {
                                            formatDisk.checked = true
                                        }
                                        onRejected:{
                                            formatDisk.checked = false
                                        }
                                    }

                                    onClicked:{
                                        if (true === formatDisk.checked) {
                                            messageDialog.visible = true
                                        }
                                    }
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
                                                bisoMode.checked,
                                                formatDisk.checked)
                                    if (0 === result) {
                                        //make the iso/usb selector invisible
                                        firstStep.visible = false
                                        secondStep.visible = true
                                        process.source = "qrc:/image/process-active.gif"
                                        isoAnimation.running = true
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
                                width: usbCreator.lableMaxWidth
                                wrapMode: Text.Wrap
                                text: qsTr("<font color='#ffffff'><br>Please <font color='#ebab4c'>DO NOT</font> remove the USB flash drive or shutdown while file is writing.</br></font>" )
                                font.pixelSize: 12
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
                                    isoAnimation.running = false
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
                                text: qsTr("<br><font color='#057aff'>Congratulations!</font></br><br><font color='#ffffff'>Deepin OS Startup Disk creates successfully.</font></br>")
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
                                    text: qsTr("Quit")
                                    width: 100
                                    anchors.verticalCenter: parent.verticalCenter
                                    //anchors.horizontalCenter: parent.horizontalCenter
                                    onClicked: {
                                        usbCreatorUI.close()
                                    }
                                }
                                DTransparentButton {
                                    text: qsTr("Reboot")
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
