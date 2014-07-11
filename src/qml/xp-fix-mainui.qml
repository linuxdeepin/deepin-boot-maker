import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.1
import QtQuick.Window 2.0
import Qt.labs.folderlistmodel 2.1
import QtQuick.Controls.Styles 1.1
import Deepin.Widgets 1.0
import com.deepin.usbcreator 1.0
import MainUI 1.0

DWindow {
    id: usbCreatorUI
    flags: /*Qt.FramelessWindowHint |*/ Qt.Window | Qt.WindowMaximizeButtonHint | Qt.WindowSystemMenuHint
    color: "transparent"
    width: 660
    height: 420

    DDragableArea {
        window: usbCreatorUI
        width: usbCreatorUI.width
        height: usbCreatorUI.height
    }

    UsbCreatorFrame {
        id: windowFrame
        frameRadius: 0
        shadowRadius: 0
    }
}
