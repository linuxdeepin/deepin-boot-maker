/*****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtQuick.Dialogs module of Qt Toolkit.
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
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Dialogs.Private 1.1
import QtQuick.Layouts 1.1
import QtQuick.Window 2.1
import "qml"

AbstractFontDialog {
    id: root

    property alias font: content.externalFont
    property alias currentFont: content.font

    Rectangle {
        id: content
        SystemPalette { id: palette }

        property int maxSize: Math.min(Screen.desktopAvailableWidth, Screen.desktopAvailableHeight)
        implicitWidth: Math.min(maxSize, Math.max(Screen.pixelDensity * 60, mainLayout.implicitWidth + outerSpacing * 2))
        implicitHeight: Math.min(maxSize, Math.max(Screen.pixelDensity * 40, mainLayout.implicitHeight + outerSpacing * 2))
        property real spacing: 6
        property real outerSpacing: 12
        color: palette.window

        property font font: Qt.font({ family: "Helvetica", pointSize: 24, weight: Font.Normal })
        property font externalFont
        property string writingSystem
        property string writingSystemSample
        property var pointSizes

        onExternalFontChanged: {
            if (Component.status != Component.Ready)
                return

            if (content.font != content.externalFont) {
                font = externalFont
                wsComboBox.reset()
                fontListView.reset()
                weightListView.reset()
            }
        }

        Component.onCompleted: externalFontChanged()

        onWritingSystemSampleChanged: { sample.text = writingSystemSample; }

        Keys.onPressed: {
            event.accepted = true
            switch (event.key) {
            case Qt.Key_Return:
            case Qt.Key_Select:
                updateUponAccepted()
                break
            case Qt.Key_Escape:
            case Qt.Key_Back:
                reject()
                break
            default:
                // do nothing
                event.accepted = false
                break
            }
        }

        function updateUponAccepted() {
            root.font = content.font
            root.accept()
        }

        ColumnLayout {
            id: mainLayout
            anchors { fill: parent; margins: content.outerSpacing }
            spacing: content.spacing

            GridLayout {
                columnSpacing: content.spacing; rowSpacing: content.spacing
                columns: 3

                Label { id: fontNameLabel; Layout.fillWidth: true; text: qsTr("Font"); font.bold: true }
                Label { id: weightLabel; text: qsTr("Weight"); font.bold: true }
                Label { id: sizeLabel; text: qsTr("Size"); font.bold: true }
                TableView {
                    id: fontListView
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumWidth: fontColumn.width + content.outerSpacing
                    headerVisible: false
                    function reset() {
                        fontModel.findIndex()
                        content.pointSizes = fontModel.pointSizes()
                        fontModel.findPointSizesIndex()
                    }
                    TableViewColumn{ id: fontColumn; role: "family"; title: qsTr("Font Family") }
                    model: FontListModel {
                        id: fontModel
                        scalableFonts: root.scalableFonts
                        nonScalableFonts: root.nonScalableFonts
                        monospacedFonts: root.monospacedFonts
                        proportionalFonts: root.proportionalFonts
                        Component.onCompleted: fontListView.reset()
                        onModelReset: { findIndex(); }
                        function findIndex() {
                            fontListView.selection.clear()
                            if (fontModel.count <= 0 || fontListView.rowCount <= 0)
                                return

                            var currentRow = 0
                            if (content.font.family != "") {
                                for (var i = 0; i < fontModel.count; ++i) {
                                    if (content.font.family == fontModel.get(i).family) {
                                        currentRow = i
                                        break
                                    }
                                }
                            }
                            content.font.family = fontModel.get(currentRow).family
                            fontListView.selection.select(currentRow)
                            fontListView.positionViewAtRow(currentRow, ListView.Contain)
                            fontListView.clicked(currentRow)
                        }
                        function findPointSizesIndex() {
                            pointSizesListView.selection.clear()
                            if (content.pointSizes.length <= 0 || pointSizesListView.rowCount <= 0)
                                return

                            var currentRow = 0
                            for (var i = 0; i < content.pointSizes.length; ++i) {
                                if (content.font.pointSize == content.pointSizes[i]) {
                                    currentRow = i
                                    break
                                }
                            }
                            content.font.pointSize = content.pointSizes[currentRow]
                            pointSizesListView.selection.select(currentRow)
                            pointSizesListView.positionViewAtRow(currentRow, ListView.Contain)
                            pointSizesListView.clicked(currentRow)
                        }
                    }
                    onClicked: {
                        if (row == -1)
                            return
                        content.font.family = fontModel.get(row).family
                        positionViewAtRow(row, ListView.Contain)
                    }
                }
                TableView {
                    id: weightListView
                    implicitWidth: (Component.status == Component.Ready) ? (weightColumn.width + content.outerSpacing) : (100)
                    Layout.fillHeight: true
                    Component.onCompleted: resizeColumnsToContents();
                    headerVisible: false
                    function reset() {
                        weightModel.findIndex()
                    }
                    TableViewColumn{ id: weightColumn; role: "name"; title: qsTr("Weight") }
                    model: ListModel {
                        id: weightModel
                        ListElement {
                            name: "Light"
                            weight: Font.Light
                        }
                        ListElement {
                            name: "Normal"
                            weight: Font.Normal
                        }
                        ListElement {
                            name: "DemiBold"
                            weight: Font.DemiBold
                        }
                        ListElement {
                            name: "Bold"
                            weight: Font.Bold
                        }
                        ListElement {
                            name: "Black"
                            weight: Font.Black
                        }
                        Component.onCompleted: weightListView.reset()
                        function findIndex() {
                            var currentRow = 1
                            for (var i = 0; i < weightModel.count; ++i) {
                                if (content.font.weight == weightModel.get(i).weight) {
                                    currentRow = i
                                    break
                                }
                            }
                            content.font.weight = weightModel.get(currentRow).family
                            weightListView.selection.select(currentRow)
                            weightListView.positionViewAtRow(currentRow, ListView.Contain)
                            weightListView.clicked(currentRow)
                        }
                    }
                    onClicked: {
                        if (row == -1)
                            return
                        content.font.weight = weightModel.get(row).weight
                        positionViewAtRow(row, ListView.Contain)
                    }
                }
                ColumnLayout {
                    SpinBox {
                        id: pointSizeSpinBox;
                        implicitWidth: (Component.status == Component.Ready) ? (psColumn.width + content.outerSpacing) : (80)
                        value: content.font.pointSize
                        decimals: 0
                        minimumValue: 1
                        maximumValue: 512
                        onValueChanged: {
                            content.font.pointSize = Number(value);
                            updatePointSizesIndex();
                        }
                        function updatePointSizesIndex() {
                            pointSizesListView.selection.clear()
                            if (content.pointSizes.length <= 0 || pointSizesListView.rowCount <= 0)
                                return
                            var currentRow = -1
                            for (var i = 0; i < content.pointSizes.length; ++i) {
                                if (content.font.pointSize == content.pointSizes[i]) {
                                    currentRow = i
                                    break
                                }
                            }
                            if (currentRow < 0)
                                return
                            content.font.pointSize = content.pointSizes[currentRow]
                            pointSizesListView.selection.select(currentRow)
                            pointSizesListView.positionViewAtRow(currentRow, ListView.Contain)
                            pointSizesListView.clicked(currentRow)
                        }
                    }
                    TableView {
                        id: pointSizesListView
                        Layout.fillHeight: true
                        headerVisible: false
                        implicitWidth: (Component.status == Component.Ready) ? (psColumn.width + content.outerSpacing) : (80)
                        Component.onCompleted: resizeColumnsToContents();
                        TableViewColumn{ id: psColumn; role: ""; title: qsTr("Size") }
                        model: content.pointSizes
                        onClicked: {
                            if (row == -1)
                                return
                            content.font.pointSize = content.pointSizes[row]
                            pointSizeSpinBox.value = content.pointSizes[row]
                            positionViewAtRow(row, ListView.Contain)
                        }
                    }
                }
            }

            RowLayout {
                spacing: content.spacing
                Layout.fillHeight: false
                ColumnLayout {
                    spacing: content.spacing
                    Layout.rowSpan: 3
                    Label { id: optionsLabel; text: qsTr("Style"); font.bold: true }
                    CheckBox {
                        id: italicCheckBox
                        text: qsTr("Italic")
                        checked: content.font.italic
                        onClicked: { content.font.italic = italicCheckBox.checked }
                    }
                    CheckBox {
                        id: underlineCheckBox
                        text: qsTr("Underline")
                        checked: content.font.underline
                        onClicked: { content.font.underline = underlineCheckBox.checked }
                    }
                    CheckBox {
                        id: overlineCheckBox
                        text: qsTr("Overline")
                        checked: content.font.overline
                        onClicked: { content.font.overline = overlineCheckBox.checked }
                    }
                    CheckBox {
                        id: strikeoutCheckBox
                        text: qsTr("Strikeout")
                        checked: content.font.strikeout
                        onClicked: { content.font.strikeout = strikeoutCheckBox.checked }
                    }
                    Item { Layout.fillHeight: true; } //spacer
                    Label { id: writingSystemLabel; text: qsTr("Writing System"); font.bold: true }
                    ComboBox {
                        id: wsComboBox
                        function reset() {
                            if (wsModel.count > 0) {
                                currentIndex = 0
                            }
                        }
                        textRole: "name"
                        model: WritingSystemListModel {
                            id: wsModel
                            Component.onCompleted: wsComboBox.reset()
                        }
                        onCurrentIndexChanged: {
                            if (currentIndex == -1)
                                return

                            content.writingSystem = wsModel.get(currentIndex).name
                            fontModel.writingSystem = content.writingSystem
                            content.writingSystemSample = wsModel.get(currentIndex).sample
                            fontListView.reset()
                        }
                    }
                }

                ColumnLayout {
                    Layout.rowSpan: 3
                    spacing: content.spacing
                    Layout.columnSpan: 2
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Label { id: sampleLabel; text: qsTr("Sample"); font.bold: true }

                    Rectangle {
                        clip: true
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        implicitWidth: Math.min(360, sample.implicitWidth + parent.spacing)
                        implicitHeight: Math.min(240, sample.implicitHeight + parent.spacing)
                        color: "white"
                        border.color: "#999"
                        TextInput {
                            id: sample
                            activeFocusOnTab: true
                            Accessible.name: text
                            Accessible.role: Accessible.EditableText
                            anchors.centerIn: parent
                            font: content.font
                            onFocusChanged: if (!focus && sample.text == "") sample.text = content.writingSystemSample
                        }
                    }
                }
            }

            RowLayout {
                id: buttonRow
                Layout.columnSpan: 3
                spacing: content.spacing
                Item { Layout.fillWidth: true; } //spacer
                Button {
                    text: qsTr("Cancel")
                    onClicked: root.reject()
                }
                Button {
                    text: qsTr("OK")
                    onClicked: {
                        content.updateUponAccepted()
                    }
                }
            }
        }
    }
}

