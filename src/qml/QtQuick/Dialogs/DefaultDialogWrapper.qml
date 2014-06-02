/*****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
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
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.1
import "qml"

AbstractDialog {
    id: root
    default property alias data: defaultContentItem.data

    Rectangle {
        id: content
        property real spacing: 6
        property real outerSpacing: 12
        property real buttonsRowImplicitWidth: minimumWidth
        property bool buttonsInSingleRow: defaultContentItem.width >= buttonsRowImplicitWidth
        property real minimumHeight: implicitHeight
        property real minimumWidth: Screen.pixelDensity * 50
        implicitHeight: defaultContentItem.implicitHeight + spacing + outerSpacing * 2 + buttonsRight.implicitHeight
        implicitWidth: Math.min(Screen.desktopAvailableWidth * 0.9, Math.max(
            defaultContentItem.implicitWidth, buttonsRowImplicitWidth, Screen.pixelDensity * 50) + outerSpacing * 2);
        color: palette.window
        focus: root.visible
        Keys.onPressed: {
            event.accepted = true
            switch (event.key) {
                case Qt.Key_Escape:
                case Qt.Key_Back:
                    reject()
                    break
                case Qt.Key_Enter:
                case Qt.Key_Return:
                    accept()
                    break
                default:
                    event.accepted = false
            }
        }

        SystemPalette { id: palette }

        Item {
            id: defaultContentItem
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
                margins: content.outerSpacing
            }
            implicitHeight: childrenRect.height
        }

        Flow {
            id: buttonsLeft
            spacing: content.spacing
            anchors {
                left: parent.left
                bottom: content.buttonsInSingleRow ? parent.bottom : buttonsRight.top
                margins: content.outerSpacing
            }

            Repeater {
                id: buttonsLeftRepeater
                Button {
                    text: (buttonsLeftRepeater.model && buttonsLeftRepeater.model[index] ? buttonsLeftRepeater.model[index].text : index)
                    onClicked: root.click(buttonsLeftRepeater.model[index].standardButton)
                }
            }

            Button {
                id: moreButton
                text: "Show Details..."
                visible: false
            }
        }

        Flow {
            id: buttonsRight
            spacing: content.spacing
            layoutDirection: Qt.RightToLeft
            anchors {
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                margins: content.outerSpacing
            }

            Repeater {
                id: buttonsRightRepeater
                // TODO maybe: insert gaps if the button requires it (destructive buttons only)
                Button {
                    text: (buttonsRightRepeater.model && buttonsRightRepeater.model[index] ? buttonsRightRepeater.model[index].text : index)
                    onClicked: root.click(buttonsRightRepeater.model[index].standardButton)
                }
            }
        }
    }
    function setupButtons() {
        buttonsLeftRepeater.model = root.__standardButtonsLeftModel()
        buttonsRightRepeater.model = root.__standardButtonsRightModel()
        if (!buttonsRightRepeater.model || buttonsRightRepeater.model.length < 2)
            return;
        var calcWidth = 0;

        function calculateForButton(i, b) {
            var buttonWidth = b.implicitWidth;
            if (buttonWidth > 0) {
                if (i > 0)
                    buttonWidth += content.spacing
                calcWidth += buttonWidth
            }
        }

        for (var i = 0; i < buttonsRight.visibleChildren.length; ++i)
            calculateForButton(i, buttonsRight.visibleChildren[i])
        content.minimumWidth = calcWidth + content.outerSpacing * 2
        for (i = 0; i < buttonsLeft.visibleChildren.length; ++i)
            calculateForButton(i, buttonsLeft.visibleChildren[i])
        content.buttonsRowImplicitWidth = calcWidth + content.spacing
    }
    onStandardButtonsChanged: setupButtons()
    Component.onCompleted: setupButtons()
}
