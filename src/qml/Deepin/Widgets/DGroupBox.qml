import QtQuick 2.2
import QtQuick.Controls 1.1
import Deepin.Widgets 1.0

GroupBox {
    id: g_box
    title: " "
    property string tTitle: ""
    
    DssH2 {
        text: g_box.tTitle
        parent: g_box
    }
}