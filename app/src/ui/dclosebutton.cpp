#include "dclosebutton.h"

DCloseButton::DCloseButton(QWidget *parent) :
    QPushButton(parent)
{
    QPixmap pixmap (":/ui/images/window_close_normal.png");
    this->setFixedSize(pixmap.size());

    QString style = "DCloseButton{"
        "background:url(:/ui/images/window_close_normal.png);"
        "border:0px;"
    "}"
    "DCloseButton:hover{"
        "background:url(:/ui/images/window_close_hover.png);"
        "border:0px;"
    "}"
    "DCloseButton:pressed{"
        "background:url(:/ui/images/window_close_press.png);"
        "border:0px;"
    "}";
    this->setStyleSheet(style);

    setFocusPolicy(Qt::NoFocus);
}
