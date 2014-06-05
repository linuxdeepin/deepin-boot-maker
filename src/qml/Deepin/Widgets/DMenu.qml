/****************************************************************************
**
**  Copyright (C) 2011~2014 Deepin, Inc.
**                2011~2014 Kaisheng Ye
**
**  Author:     Kaisheng Ye <kaisheng.ye@gmail.com>
**  Maintainer: Kaisheng Ye <kaisheng.ye@gmail.com>
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

import QtQuick 2.1
import QtQuick.Window 2.1

DPopupWindow {
    id: menuPopupWindow
    property int frameEdge: menuFrame.shadowRadius + menuFrame.frameRadius
    property int minWidth: 30
    property real posX: 0
    property real posY: 0

    x: posX - 28
    y: posY - 12

    width: minWidth + 24
    height: completeViewBox.height + 32

    property alias currentIndex: completeView.currentIndex
    property var labels
    visible: false

    signal menuSelect(int index)

    DWindowFrame {
        id: menuFrame
        anchors.fill: parent

        Item {
            id: completeViewBox
            anchors.centerIn: parent
            width: parent.width - 6
            height: childrenRect.height

            ListView {
                id: completeView
                width: parent.width
                height: childrenRect.height
                model: labels
                delegate: DMenuItem {
                    text: labels[index]
                    onSelectAction:{
                        menuPopupWindow.visible = false
                        menuPopupWindow.menuSelect(index)
                    }
                }
                clip: true
                interactive: false
            }
        }

    }

}
