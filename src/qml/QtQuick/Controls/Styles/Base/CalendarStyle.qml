/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Quick Controls module of the Qt Toolkit.
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
****************************************************************************/

import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Private 1.0

/*!
    \qmltype CalendarStyle
    \inqmlmodule QtQuick.Controls.Styles
    \since 5.3
    \ingroup controlsstyling
    \brief Provides custom styling for \l Calendar

    \section2 Component Map

    \image calendarstyle-components-week-numbers.png

    The calendar has the following styleable components:

    \table
        \row \li \image square-white.png
            \li \l background
            \li Fills the entire control.
        \row \li \image square-yellow.png
            \li \l navigationBar
            \li
        \row \li \image square-green.png
            \li \l dayOfWeekDelegate
            \li One instance per day of week.
        \row \li \image square-red.png
            \li \l weekNumberDelegate
            \li One instance per week.
        \row \li \image square-blue.png
            \li \l dayDelegate
            \li One instance per day of month.
    \endtable

    \section2 Custom Style Example
    \qml
    Calendar {
        anchors.centerIn: parent

        style: CalendarStyle {
            gridVisible: false
            dayDelegate: Rectangle {
                gradient: Gradient {
                    GradientStop {
                        position: 0.00
                        color: styleData.selected ? "#111" : (styleData.visibleMonth && styleData.valid ? "#444" : "#666");
                    }
                    GradientStop {
                        position: 1.00
                        color: styleData.selected ? "#444" : (styleData.visibleMonth && styleData.valid ? "#111" : "#666");
                    }
                    GradientStop {
                        position: 1.00
                        color: styleData.selected ? "#777" : (styleData.visibleMonth && styleData.valid ? "#111" : "#666");
                    }
                }

                Label {
                    text: styleData.date.getDate()
                    anchors.centerIn: parent
                    color: styleData.valid ? "white" : "grey"
                }

                Rectangle {
                    width: parent.width
                    height: 1
                    color: "#555"
                    anchors.bottom: parent.bottom
                }

                Rectangle {
                    width: 1
                    height: parent.height
                    color: "#555"
                    anchors.right: parent.right
                }
            }
        }
    }
    \endqml
*/

Style {
    id: calendarStyle

    /*!
        The Calendar attached to this style.
    */
    property Calendar control: __control

    /*!
        The color of the grid lines.
    */
    property color gridColor: "#d3d3d3"

    /*!
        This property determines the visibility of the grid.

        The default value is \c true.
    */
    property bool gridVisible: true

    /*!
        \internal

        The width of each grid line.
    */
    property real __gridLineWidth: 1

    function __cellRectAt(index) {
        return CalendarUtils.cellRectAt(index, control.__panel.columns, control.__panel.rows,
            control.__panel.availableWidth, control.__panel.availableHeight);
    }

    function __isValidDate(date) {
        return date !== undefined
            && date.getTime() >= control.minimumDate.getTime()
            && date.getTime() <= control.maximumDate.getTime();
    }

    /*!
        The background of the calendar.
    */
    property Component background: Rectangle {
        color: "#fff"
    }

    /*!
        The navigation bar of the calendar.

        Styles the bar at the top of the calendar that contains the
        next month/previous month buttons and the selected date label.
    */
    property Component navigationBar: Rectangle {
        height: 41
        color: "#f9f9f9"

        Rectangle {
            color: Qt.rgba(1,1,1,0.6)
            height: 1
            width: parent.width
        }

        Rectangle {
            anchors.bottom: parent.bottom
            height: 1
            width: parent.width
            color: "#ddd"
        }
        HoverButton {
            id: previousMonth
            width: parent.height
            height: width
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            source: "images/leftanglearrow.png"
            onClicked: control.showPreviousMonth()
        }
        Label {
            id: dateText
            text: styleData.title
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 14
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: previousMonth.right
            anchors.leftMargin: 2
            anchors.right: nextMonth.left
            anchors.rightMargin: 2
        }
        HoverButton {
            id: nextMonth
            width: parent.height
            height: width
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            source: "images/rightanglearrow.png"
            onClicked: control.showNextMonth()
        }
    }

    /*!
        The delegate that styles each date in the calendar.

        The properties provided to each delegate are:
        \table
            \row \li readonly property date \b styleData.date
                \li The date this delegate represents.
            \row \li readonly property bool \b styleData.selected
                \li \c true if this is the selected date.
            \row \li readonly property int \b styleData.index
                \li The index of this delegate.
            \row \li readonly property bool \b styleData.valid
                \li \c true if this date is greater than or equal to than \l {Calendar::minimumDate}{minimumDate} and
                    less than or equal to \l {Calendar::maximumDate}{maximumDate}.
            \row \li readonly property bool \b styleData.today
                \li \c true if this date is equal to today's date.
            \row \li readonly property bool \b styleData.visibleMonth
                \li \c true if the month in this date is the visible month.
            \row \li readonly property bool \b styleData.hovered
                \li \c true if the mouse is over this cell.
                    \note This property is \c true even when the mouse is hovered over an invalid date.
            \row \li readonly property bool \b styleData.pressed
                \li \c true if the mouse is pressed on this cell.
                    \note This property is \c true even when the mouse is pressed on an invalid date.
        \endtable
    */
    property Component dayDelegate: Rectangle {
        anchors.fill: parent
        anchors.margins: styleData.selected ? -1 : 0
        color: styleData.date !== undefined && styleData.selected ? selectedDateColor : "transparent"
        readonly property color sameMonthDateTextColor: "#444"
        readonly property color selectedDateColor: Qt.platform.os === "osx" ? "#3778d0" : __syspal.highlight
        readonly property color selectedDateTextColor: "white"
        readonly property color differentMonthDateTextColor: "#bbb"
        readonly property color invalidDateColor: "#dddddd"
        Label {
            id: dayDelegateText
            text: styleData.date.getDate()
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignRight
            font.pixelSize: Math.min(parent.height/3, parent.width/3)
            color: {
                var theColor = invalidDateColor;
                if (styleData.valid) {
                    // Date is within the valid range.
                    theColor = styleData.visibleMonth ? sameMonthDateTextColor : differentMonthDateTextColor;
                    if (styleData.selected)
                        theColor = selectedDateTextColor;
                }
                theColor;
            }
        }
    }

    /*!
        The delegate that styles each weekday.
    */
    property Component dayOfWeekDelegate: Rectangle {
        color: gridVisible ? "#fcfcfc" : "transparent"
        Label {
            text: control.__locale.dayName(styleData.dayOfWeek, control.dayOfWeekFormat)
            anchors.centerIn: parent
        }
    }

    /*!
        The delegate that styles each week number.
    */
    property Component weekNumberDelegate: Rectangle {
        Label {
            text: styleData.weekNumber
            anchors.centerIn: parent
            color: "#444"
        }
    }

    /*! \internal */
    property Component panel: Item {
        id: panelItem

        implicitWidth: 200
        implicitHeight: 200

        property alias navigationBarItem: navigationBarLoader.item

        readonly property real dayOfWeekHeaderRowHeight: 40

        readonly property int weeksToShow: 6
        readonly property int rows: weeksToShow
        readonly property int columns: CalendarUtils.daysInAWeek

        // The combined available width and height to be shared amongst each cell.
        readonly property real availableWidth: (viewContainer.width - (control.frameVisible ? 2 * __gridLineWidth : 0))
        readonly property real availableHeight: (viewContainer.height - (control.frameVisible ? 2 * __gridLineWidth : 0))

        property int hoveredCellIndex: -1
        property int pressedCellIndex: -1

        Loader {
            id: backgroundLoader
            anchors.fill: parent
            sourceComponent: background
        }

        Loader {
            id: navigationBarLoader
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.margins: control.frameVisible ? 1 : 0
            sourceComponent: navigationBar

            property QtObject styleData: QtObject {
                readonly property string title: control.__locale.standaloneMonthName(control.visibleMonth)
                    + new Date(control.visibleYear, control.visibleMonth, 1).toLocaleDateString(control.__locale, " yyyy")
            }
        }

        Row {
            id: dayOfWeekHeaderRow
            anchors.top: navigationBarLoader.bottom
            anchors.left: parent.left
            anchors.leftMargin: (control.weekNumbersVisible ? weekNumbersItem.width : 0)
            anchors.right: parent.right
            height: dayOfWeekHeaderRowHeight

            Repeater {
                id: repeater
                model: CalendarHeaderModel {
                    locale: control.__locale
                }
                Loader {
                    id: dayOfWeekDelegateLoader
                    sourceComponent: dayOfWeekDelegate
                    width: __cellRectAt(index).width
                    height: dayOfWeekHeaderRow.height

                    readonly property var __dayOfWeek: dayOfWeek

                    property QtObject styleData: QtObject {
                        readonly property alias dayOfWeek: dayOfWeekDelegateLoader.__dayOfWeek
                    }
                }
            }
        }

        Row {
            id: gridRow
            width: weekNumbersItem.width + viewContainer.width
            height: viewContainer.height
            anchors.top: dayOfWeekHeaderRow.bottom

            Item {
                id: weekNumbersItem
                visible: control.weekNumbersVisible
                width: 30
                height: viewContainer.height
                Repeater {
                    id: weekNumberRepeater
                    model: panelItem.weeksToShow

                    Loader {
                        id: weekNumberDelegateLoader
                        y: __cellRectAt(index * panelItem.columns).y + (gridVisible ? __gridLineWidth : 0)
                        width: weekNumbersItem.width
                        height: __cellRectAt(index * panelItem.columns).height - (control.frameVisible ? __gridLineWidth : 0)
                        sourceComponent: weekNumberDelegate

                        readonly property int __index: index
                        property int __weekNumber: control.__model.weekNumberAt(index)

                        Connections {
                            target: control
                            onVisibleMonthChanged: __weekNumber = control.__model.weekNumberAt(index)
                            onVisibleYearChanged: __weekNumber = control.__model.weekNumberAt(index)
                        }

                        Connections {
                            target: control.__model
                            onCountChanged: __weekNumber = control.__model.weekNumberAt(index)
                        }

                        property QtObject styleData: QtObject {
                            readonly property alias index: weekNumberDelegateLoader.__index
                            readonly property int weekNumber: weekNumberDelegateLoader.__weekNumber
                        }
                    }
                }
                Rectangle {
                    anchors.topMargin: - navigationBarLoader.height
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom

                    width: 1
                    anchors.rightMargin: -1
                    anchors.right: parent.right
                    color: gridColor
                }

            }

            // Contains the grid lines and the grid itself.
            Item {
                id: viewContainer
                width: panelItem.width - (control.weekNumbersVisible ? weekNumbersItem.width : 0)
                height: panelItem.height - navigationBarLoader.height - dayOfWeekHeaderRow.height

                Repeater {
                    id: verticalGridLineRepeater
                    model: panelItem.columns - 1
                    delegate: Rectangle {
                        // The last line will be an invalid index, so we must handle it
                        x: index < panelItem.columns
                           ? __cellRectAt(index + 1).x
                           : __cellRectAt(panelItem.columns).x + __cellRectAt(panelItem.columns).width
                        y: 0
                        width: __gridLineWidth
                        height: viewContainer.height
                        color: gridColor
                        visible: gridVisible
                    }
                }

                Repeater {
                    id: horizontalGridLineRepeater
                    model: panelItem.rows
                    delegate: Rectangle {
                        x: 0
                        // The last line will be an invalid index, so we must handle it
                        y: index < panelItem.columns - 1
                            ? __cellRectAt(index * panelItem.columns).y
                            : __cellRectAt((panelItem.rows - 1) * panelItem.columns).y + __cellRectAt((panelItem.rows - 1) * panelItem.columns).height
                        width: viewContainer.width
                        height: __gridLineWidth
                        color: gridColor
                        visible: gridVisible
                    }
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent

                    hoverEnabled: true

                    function cellIndexAt(mouseX, mouseY) {
                        var viewContainerPos = viewContainer.mapFromItem(mouseArea, mouseX, mouseY);
                        var child = viewContainer.childAt(viewContainerPos.x, viewContainerPos.y);
                        // In the tests, the mouseArea sometimes gets picked instead of the cells,
                        // probably because stuff is still loading. To be safe, we check for that here.
                        return child && child !== mouseArea ? child.__index : -1;
                    }

                    onEntered: {
                        hoveredCellIndex = cellIndexAt(mouseX, mouseY);
                        if (hoveredCellIndex === undefined) {
                            hoveredCellIndex = cellIndexAt(mouseX, mouseY);
                        }

                        var date = view.model.dateAt(hoveredCellIndex);
                        if (__isValidDate(date)) {
                            control.hovered(date);
                        }
                    }

                    onExited: {
                        hoveredCellIndex = -1;
                    }

                    onPositionChanged: {
                        var indexOfCell = cellIndexAt(mouse.x, mouse.y);
                        var previousHoveredCellIndex = hoveredCellIndex;
                        hoveredCellIndex = indexOfCell;
                        if (indexOfCell !== -1) {
                            var date = view.model.dateAt(indexOfCell);
                            if (__isValidDate(date)) {
                                if (hoveredCellIndex !== previousHoveredCellIndex)
                                    control.hovered(date);

                                if (pressed && date.getTime() !== control.selectedDate.getTime()) {
                                    control.selectedDate = date;
                                    pressedCellIndex = indexOfCell;
                                    control.pressed(date);
                                }
                            }
                        }
                    }

                    onPressed: {
                        var indexOfCell = cellIndexAt(mouse.x, mouse.y);
                        if (indexOfCell !== -1) {
                            var date = view.model.dateAt(indexOfCell);
                            pressedCellIndex = indexOfCell;
                            if (__isValidDate(date)) {
                                control.selectedDate = date;
                                control.pressed(date);
                            }
                        }
                    }

                    onReleased: {
                        var indexOfCell = cellIndexAt(mouse.x, mouse.y);
                        if (indexOfCell !== -1) {
                            // The cell index might be valid, but the date has to be too. We could let the
                            // selected date validation take care of this, but then the selected date would
                            // change to the earliest day if a day before the minimum date is clicked, for example.
                            var date = view.model.dateAt(indexOfCell);
                            if (__isValidDate(date)) {
                                control.released(date);
                            }
                        }
                        pressedCellIndex = -1;
                    }

                    onClicked: {
                        var indexOfCell = cellIndexAt(mouse.x, mouse.y);
                        if (indexOfCell !== -1) {
                            var date = view.model.dateAt(indexOfCell);
                            if (__isValidDate(date))
                                control.clicked(date);
                        }
                    }

                    onDoubleClicked: {
                        var indexOfCell = cellIndexAt(mouse.x, mouse.y);
                        if (indexOfCell !== -1) {
                            var date = view.model.dateAt(indexOfCell);
                            if (__isValidDate(date))
                                control.doubleClicked(date);
                        }
                    }
                }

                Connections {
                    target: control
                    onSelectedDateChanged: view.selectedDateChanged()
                }

                Repeater {
                    id: view

                    property int currentIndex: -1

                    model: control.__model

                    Component.onCompleted: selectedDateChanged()

                    function selectedDateChanged() {
                        if (model !== undefined && model.locale !== undefined) {
                            currentIndex = model.indexAt(control.selectedDate);
                        }
                    }

                    delegate: Loader {
                        id: delegateLoader

                        x: __cellRectAt(index).x + (gridVisible ? __gridLineWidth : 0)
                        y: __cellRectAt(index).y + (gridVisible ? __gridLineWidth : 0)
                        width: __cellRectAt(index).width - (gridVisible ? __gridLineWidth : 0)
                        height: __cellRectAt(index).height - (gridVisible ? __gridLineWidth : 0)
                        z: styleData.selected ? 1 : 0
                        sourceComponent: dayDelegate

                        readonly property int __index: index
                        readonly property date __date: date
                        // We rely on the fact that an invalid QDate will be converted to a Date
                        // whose year is -4713, which is always an invalid date since our
                        // earliest minimum date is the year 1.
                        readonly property bool valid: __isValidDate(date)

                        property QtObject styleData: QtObject {
                            readonly property alias index: delegateLoader.__index
                            readonly property bool selected: control.selectedDate.getTime() === date.getTime()
                            readonly property alias date: delegateLoader.__date
                            readonly property bool valid: delegateLoader.valid
                            // TODO: this will not be correct if the app is running when a new day begins.
                            readonly property bool today: date.getTime() === new Date().setHours(0, 0, 0, 0)
                            readonly property bool visibleMonth: date.getMonth() === control.visibleMonth
                            readonly property bool hovered: panelItem.hoveredCellIndex == index
                            readonly property bool pressed: panelItem.pressedCellIndex == index
                            // todo: pressed property here, clicked and doubleClicked in the control itself
                        }
                    }
                }
            }
        }
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.color: gridColor
            visible: control.frameVisible
        }

    }
}
