import QtQuick 2.1
import Deepin.Widgets 1.0

DImageCheckButton {
    inactivatedNormalImage: "images/multiselect_inactive_normal.png"
    inactivatedHoverImage: "images/multiselect_inactive_hover.png"
    inactivatedPressImage: "images/multiselect_active_normal.png"

    activatedNormalImage: inactivatedPressImage
    activatedHoverImage: inactivatedPressImage
    activatedPressImage: inactivatedPressImage
}
