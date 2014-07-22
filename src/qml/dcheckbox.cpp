#include "dcheckbox.h"

DCheckBox::DCheckBox(QWidget *parent) :
    QCheckBox(parent)
{
    QCheckBox {
         spacing: 5px;
     }

     QCheckBox::indicator {
         width: 13px;
         height: 13px;
     }

     QCheckBox::indicator:unchecked {
         image: url(:/images/checkbox_unchecked.png);
     }

     QCheckBox::indicator:unchecked:hover {
         image: url(:/images/checkbox_unchecked_hover.png);
     }

     QCheckBox::indicator:unchecked:pressed {
         image: url(:/images/checkbox_unchecked_pressed.png);
     }

     QCheckBox::indicator:checked {
         image: url(:/images/checkbox_checked.png);
     }

     QCheckBox::indicator:checked:hover {
         image: url(:/images/checkbox_checked_hover.png);
     }

     QCheckBox::indicator:checked:pressed {
         image: url(:/images/checkbox_checked_pressed.png);
     }

     QCheckBox::indicator:indeterminate:hover {
         image: url(:/images/checkbox_indeterminate_hover.png);
     }

     QCheckBox::indicator:indeterminate:pressed {
         image: url(:/images/checkbox_indeterminate_pressed.png);
     }
}
