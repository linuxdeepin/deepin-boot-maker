import QtQuick 2.1
import QtQuick.Controls 1.0

Label {
    focus: true
    property QtObject dconstants: DConstants {}
    
    color: dconstants.fgColor
}
