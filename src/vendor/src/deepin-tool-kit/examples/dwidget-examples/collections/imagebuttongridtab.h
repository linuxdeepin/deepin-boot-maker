/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef IMAGEBUTTONGRIDTAB_H
#define IMAGEBUTTONGRIDTAB_H

#include <QFrame>

class ImageButtonGridTab : public QFrame
{
    Q_OBJECT
public:
    explicit ImageButtonGridTab(QWidget *parent = 0);
    ~ImageButtonGridTab();
    void initData();
    void initUI();
    void initConnect();
signals:

public slots:
    void handleClicked(QString label);
private:
    QList<QMap<QString, QString>> m_imageInfos;
};


#endif // IMAGEBUTTONGRIDTAB_H
