import QtQuick 2.1
import QtQuick.Window 2.1
import Qt.labs.folderlistmodel 2.1

Window {
    id: rootWindow
    flags: Qt.Dialog | Qt.FramelessWindowHint
    color: "transparent"
    width: frame.width
    height: frame.height
    property var dconstants: DConstants {}

    property alias folderModel: folderModel
    property bool isVisible: false

    property int xPadding: 16
    property string currentFolder: "/"
    property bool isChooseFile: true

    signal selectAction(url fileUrl)
    signal cancelAction

    function showWindow(){
        isVisible = true
        show()
    }

    function hideWindow(){
        hide()
        isVisible = false
    }

    DWindowFrame {
        id: frame
        width: 600 + (shadowRadius + frameRadius) * 2
        height: 350 + (shadowRadius + frameRadius) * 2

        MouseArea {
            anchors.fill: parent

            property int startX
            property int startY
            property bool holdFlag
            onPressed: {
                startX = mouse.x;
                startY = mouse.y;
                holdFlag = true;
            }
            onReleased: holdFlag = false;
            onPositionChanged: {
                if (holdFlag) {
                    rootWindow.setX(rootWindow.x + mouse.x - startX)
                    rootWindow.setY(rootWindow.y + mouse.y - startY)
                }
            }
        }

        Item {
            width: parent.width - xPadding * 2
            height: parent.height - xPadding * 2
            anchors.centerIn: parent
            clip: true

            Item {
                id: locationBox
                height: 38
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter

                DssH2 {
                    id: locationLabel
                    text: "Location: "
                    anchors.verticalCenter: parent.verticalCenter
                }

                DTextInput {
                    anchors.left: locationLabel.right
                    anchors.leftMargin: 10
                    anchors.right: goUpFolder.left
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    text: currentFolder
                }

                Rectangle {
                    id: goUpFolder
                    width: 24
                    height: 24
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right

                    color: {
                        if(pressed){
                            return Qt.rgba(0, 0, 0, 0.4)
                        }
                        else{
                            if(hovered){
                                return Qt.rgba(1, 1, 1, 0.1)
                            }
                            else{
                                return "transparent"
                            }
                        }
                    }

                    property bool hovered: false
                    property bool pressed: false

                    Image {
                        id: img
                        anchors.centerIn: parent
                        source: "images/folder_up_24.png"
                        scale: 0.8
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: parent.hovered = true
                        onExited: parent.hovered = false
                        onPressed: parent.pressed = true
                        onReleased: { parent.pressed = false; parent.hovered = containsMouse; }
                        onClicked: rootWindow.currentFolder = String(folderModel.parentFolder).slice(7)
                    }
                }
            }

            Rectangle {
                id: fileListViewBox
                anchors.top: locationBox.bottom
                anchors.bottom: buttonBox.top
                anchors.bottomMargin: 10
                width: parent.width
                color: "transparent"
                border.width: 1
                border.color: dconstants.fgDarkColor
                clip: true

                ListView {
                    id: fileListView
                    anchors.fill: parent

                    FolderListModel {
                        id: folderModel
                        showDirsFirst: true
                        folder: currentFolder
                    }

                    Component {
                        id: fileDelegate
                        Item {
                            width: parent.width
                            height: 30

                            Image {
                                anchors.left: parent.left
                                anchors.leftMargin: 4
                                anchors.verticalCenter: parent.verticalCenter
                                source: fileIsDir ? "images/folder_24.png" : "images/file_24.png"
                            }

                            DLabel { 
                                text: fileName;
                                color: "white"
                                anchors.left: parent.left
                                anchors.leftMargin: 36
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    parent.ListView.view.currentIndex = index
                                }
                                onDoubleClicked: {
                                    if(fileIsDir){
                                        rootWindow.currentFolder = filePath
                                        parent.ListView.view.currentIndex = -1
                                    }
                                    else{
                                        rootWindow.selectAction(filePath)
                                        rootWindow.hideWindow()
                                    }
                                }
                            }
                        }
                    }

                    model: folderModel
                    delegate: fileDelegate
                    highlight: Rectangle{
                        width: fileListView.width
                        height: 30
                        color: Qt.rgba(0, 189/255, 1, 0.5)
                    }
                    highlightMoveDuration: 0

                    DScrollBar {
                        flickable: fileListView
                    }
                }
            }

            Row {
                id: buttonBox
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.rightMargin: 15
                spacing: 10
                DTextButton {
                    text: "Cancel"
                    onClicked: {
                        rootWindow.hideWindow()
                        rootWindow.cancelAction()
                    }
                }
                DTextButton {
                    minMiddleWidth: 48
                    text: "OK"
                    onClicked: {
                        if(fileListView.currentIndex != -1){
                            if(isChooseFile && !folderModel.isFolder(fileListView.currentIndex)){
                                rootWindow.selectAction(folderModel.get(fileListView.currentIndex, "filePath"))
                                rootWindow.hideWindow()
                            }
                            else if(!isChooseFile && folderModel.isFolder(fileListView.currentIndex)){
                                rootWindow.selectAction(folderModel.get(fileListView.currentIndex, "filePath"))
                                rootWindow.hideWindow()
                            }
                        }
                    }
                }
            }

        }
    }
}

