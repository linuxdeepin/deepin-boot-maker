#include "suggestbutton.h"

#include "widgetutil.h"

SuggestButton::SuggestButton(QWidget *parent) : DBaseButton(parent)
{
    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/SuggestButton.theme"));
    this->setFixedSize(QPixmap(":/theme/light/image/suggest_button_hover.png").size());
}
