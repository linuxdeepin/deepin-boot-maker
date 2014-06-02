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
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.1
import "qml"

AbstractMessageDialog {
    id: root

    Rectangle {
        id: content
        property real spacing: 6
        property real outerSpacing: 12
        property real buttonsRowImplicitWidth: Screen.pixelDensity * 50
        implicitHeight: contentColumn.implicitHeight + outerSpacing * 2
        onImplicitHeightChanged: root.height = implicitHeight
        implicitWidth: Math.min(Screen.desktopAvailableWidth * 0.9, Math.max(
            mainText.implicitWidth, buttonsRowImplicitWidth) + outerSpacing * 2);
        onImplicitWidthChanged: root.width = implicitWidth
        color: palette.window
        focus: root.visible
        Keys.onPressed: {
            event.accepted = true
            if (event.modifiers === Qt.ControlModifier)
                switch (event.key) {
                case Qt.Key_A:
                    detailedText.selectAll()
                    break
                case Qt.Key_C:
                    detailedText.copy()
                    break
                case Qt.Key_Period:
                    if (Qt.platform.os === "osx")
                        reject()
                    break
            } else switch (event.key) {
                case Qt.Key_Escape:
                case Qt.Key_Back:
                    reject()
                    break
                case Qt.Key_Enter:
                case Qt.Key_Return:
                    accept()
                    break
            }
        }

        Column {
            id: contentColumn
            spacing: content.spacing
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                margins: content.outerSpacing
            }

            SystemPalette { id: palette }

            Item {
                width: parent.width
                height: Math.max(icon.height, mainText.height + informativeText.height + content.spacing)
                Image {
                    id: icon
                    source: root.standardIconSource
                }

                Text {
                    id: mainText
                    anchors {
                        left: icon.right
                        leftMargin: content.spacing
                        right: parent.right
                    }
                    text: root.text
                    font.weight: Font.Bold
                    wrapMode: Text.WordWrap
                }

                Text {
                    id: informativeText
                    anchors {
                        left: icon.right
                        right: parent.right
                        top: mainText.bottom
                        leftMargin: content.spacing
                        topMargin: content.spacing
                    }
                    text: root.informativeText
                    wrapMode: Text.WordWrap
                }
            }


            Flow {
                id: buttons
                spacing: content.spacing
                layoutDirection: Qt.RightToLeft
                width: parent.width + content.outerSpacing
                x: -content.outerSpacing
                Button {
                    id: okButton
                    text: "OK"
                    onClicked: root.click(StandardButton.Ok)
                    visible: root.standardButtons & StandardButton.Ok
                }
                Button {
                    id: openButton
                    text: "Open"
                    onClicked: root.click(StandardButton.Open)
                    visible: root.standardButtons & StandardButton.Open
                }
                Button {
                    id: saveButton
                    text: "Save"
                    onClicked: root.click(StandardButton.Save)
                    visible: root.standardButtons & StandardButton.Save
                }
                Button {
                    id: saveAllButton
                    text: "Save All"
                    onClicked: root.click(StandardButton.SaveAll)
                    visible: root.standardButtons & StandardButton.SaveAll
                }
                Button {
                    id: retryButton
                    text: "Retry"
                    onClicked: root.click(StandardButton.Retry)
                    visible: root.standardButtons & StandardButton.Retry
                }
                Button {
                    id: ignoreButton
                    text: "Ignore"
                    onClicked: root.click(StandardButton.Ignore)
                    visible: root.standardButtons & StandardButton.Ignore
                }
                Button {
                    id: applyButton
                    text: "Apply"
                    onClicked: root.click(StandardButton.Apply)
                    visible: root.standardButtons & StandardButton.Apply
                }
                Button {
                    id: yesButton
                    text: "Yes"
                    onClicked: root.click(StandardButton.Yes)
                    visible: root.standardButtons & StandardButton.Yes
                }
                Button {
                    id: yesAllButton
                    text: "Yes to All"
                    onClicked: root.click(StandardButton.YesToAll)
                    visible: root.standardButtons & StandardButton.YesToAll
                }
                Button {
                    id: noButton
                    text: "No"
                    onClicked: root.click(StandardButton.No)
                    visible: root.standardButtons & StandardButton.No
                }
                Button {
                    id: noAllButton
                    text: "No to All"
                    onClicked: root.click(StandardButton.NoToAll)
                    visible: root.standardButtons & StandardButton.NoToAll
                }
                Button {
                    id: discardButton
                    text: "Discard"
                    onClicked: root.click(StandardButton.Discard)
                    visible: root.standardButtons & StandardButton.Discard
                }
                Button {
                    id: resetButton
                    text: "Reset"
                    onClicked: root.click(StandardButton.Reset)
                    visible: root.standardButtons & StandardButton.Reset
                }
                Button {
                    id: restoreDefaultsButton
                    text: "Restore Defaults"
                    onClicked: root.click(StandardButton.RestoreDefaults)
                    visible: root.standardButtons & StandardButton.RestoreDefaults
                }
                Button {
                    id: cancelButton
                    text: "Cancel"
                    onClicked: root.click(StandardButton.Cancel)
                    visible: root.standardButtons & StandardButton.Cancel
                }
                Button {
                    id: abortButton
                    text: "Abort"
                    onClicked: root.click(StandardButton.Abort)
                    visible: root.standardButtons & StandardButton.Abort
                }
                Button {
                    id: closeButton
                    text: "Close"
                    onClicked: root.click(StandardButton.Close)
                    visible: root.standardButtons & StandardButton.Close
                }
                Button {
                    id: moreButton
                    text: "Show Details..."
                    onClicked: content.state = (content.state === "" ? "expanded" : "")
                    visible: root.detailedText.length > 0
                }
                Button {
                    id: helpButton
                    text: "Help"
                    onClicked: root.click(StandardButton.Help)
                    visible: root.standardButtons & StandardButton.Help
                }
                onVisibleChildrenChanged: calculateImplicitWidth()
            }
        }

        Item {
            id: details
            width: parent.width
            implicitHeight: detailedText.implicitHeight + content.spacing
            height: 0
            clip: true

            anchors {
                left: parent.left
                right: parent.right
                top: contentColumn.bottom
                topMargin: content.spacing
                leftMargin: content.outerSpacing
                rightMargin: content.outerSpacing
            }

            Flickable {
                id: flickable
                contentHeight: detailedText.height
                anchors.fill: parent
                anchors.topMargin: content.spacing
                anchors.bottomMargin: content.outerSpacing
                TextEdit {
                    id: detailedText
                    text: root.detailedText
                    width: details.width
                    wrapMode: Text.WordWrap
                    readOnly: true
                    selectByMouse: true
                }
            }
        }

        states: [
            State {
                name: "expanded"
                PropertyChanges {
                    target: details
                    height: content.height - contentColumn.height - content.spacing - content.outerSpacing
                }
                PropertyChanges {
                    target: content
                    implicitHeight: contentColumn.implicitHeight + content.spacing * 2 +
                        detailedText.implicitHeight + content.outerSpacing * 2
                }
                PropertyChanges {
                    target: moreButton
                    text: "Hide Details"
                }
            }
        ]
    }
    function calculateImplicitWidth() {
        if (buttons.visibleChildren.length < 2)
            return;
        var calcWidth = 0;
        for (var i = 0; i < buttons.visibleChildren.length; ++i)
            calcWidth += Math.max(100, buttons.visibleChildren[i].implicitWidth) + content.spacing
        content.buttonsRowImplicitWidth = content.outerSpacing + calcWidth
    }
    Component.onCompleted: calculateImplicitWidth()
}
