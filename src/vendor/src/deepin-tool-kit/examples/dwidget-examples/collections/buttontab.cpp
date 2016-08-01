/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "buttontab.h"

#include <QDebug>
#include "dbasebutton.h"

DWIDGET_USE_NAMESPACE

ButtonTab::ButtonTab(QWidget *parent) : QLabel(parent)
{
    setStyleSheet("ButtonTab{background-color: #252627;}");

    DArrowButton * arrowButton = new DArrowButton(this);
    arrowButton->setArrowDirection(DArrowButton::ArrowDown);
    arrowButton->move(5, 5);

    DWindowMinButton * minButton = new DWindowMinButton(this);
    minButton->move(30, 5);

    DWindowMaxButton * maxButton = new DWindowMaxButton(this);
    maxButton->move(50, 5);

    DWindowRestoreButton * restoreButton = new DWindowRestoreButton(this);
    restoreButton->move(70, 5);

    DWindowCloseButton * closeButton = new DWindowCloseButton(this);
    closeButton->move(90, 5);

    DWindowOptionButton * optionButton = new DWindowOptionButton(this);
    optionButton->move(110, 5);

    //////////////////////////////////////////////////////////////--DTextButton
    DTextButton *textButton = new DTextButton("Test Text", this);
    textButton->move(5, 40);
    
    DTextButton *textComplexButton = new DTextButton("Checked Button", this);
    textComplexButton->setCheckable(true);
    textComplexButton->setChecked(true);
    textComplexButton->move(5, 75);

    //////////////////////////////////////////////////////////////--DTextButton

    DImageButton *imageButton = new DImageButton(":/images/button.png", ":/images/buttonHover.png", ":/images/buttonPress.png", this);
    imageButton->move(5, 100);
    imageButton->setChecked(true);

    DImageButton *imageButton2 = new DImageButton(this);
    imageButton2->setNormalPic(":/images/buttonHover.png");
    imageButton2->move(35, 100);

    DImageButton *checkableImageButton = new DImageButton(":/images/button.png", ":/images/buttonHover.png", ":/images/buttonPress.png", ":/images/buttonChecked.png", this);
    checkableImageButton->move(85, 100);
    connect(checkableImageButton, SIGNAL(clicked()), this, SLOT(buttonClickTest()));

    DSwitchButton *switchButton = new DSwitchButton(this);
    switchButton->move(85, 200);

    DTK_WIDGET_NAMESPACE::DLinkButton *linkBtn = new DTK_WIDGET_NAMESPACE::DLinkButton("Link Button", this);
    linkBtn->move(10, 125);

    DTK_WIDGET_NAMESPACE::DLinkButton *linkButton = new DTK_WIDGET_NAMESPACE::DLinkButton("Link Button", this);
    linkButton->move(10, 155);

    /////////////////////////////////////////////////////////////////////////////
    DTK_WIDGET_NAMESPACE::DBaseButton *bf = new DTK_WIDGET_NAMESPACE::DBaseButton(this);
    bf->setFixedSize(100, 30);
    bf->move(10, 260);
    bf->show();
}

void ButtonTab::buttonClickTest()
{
    qDebug() << "clicked";
}
