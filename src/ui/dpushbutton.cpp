#include "dpushbutton.h"

DPushButton::DPushButton(const QString& text, QWidget *parent) :
    QPushButton(text, parent)
{
    QString style = "QPushButton{"
        "color: #b4b4b4;"
        "font-size: 12px;"
        "border-image:url(:/ui/images/transparent_button_normal.png) 3 6 3 6;"
        "border-top: 3px transparent;"
        "border-bottom: 3px transparent;"
        "border-right: 6px transparent;"
        "border-left: 6px transparent;"
    "}"
    "QPushButton:hover{"
        "border-image:url(:/ui/images/transparent_button_hover.png) 3 6 3 6;"
        "border-top: 3px transparent;"
        "border-bottom: 3px transparent;"
        "border-right: 6px transparent;"
        "border-left: 6px transparent;"
    "}"
    "QPushButton:pressed{"
        "border-image:url(:/ui/images/transparent_button_press.png) 3 6 3 6;"
        "border-top: 3px transparent;"
        "border-bottom: 3px transparent;"
        "border-right: 6px transparent;"
        "border-left: 6px transparent;"
    "}";
    setStyleSheet(style);
}
