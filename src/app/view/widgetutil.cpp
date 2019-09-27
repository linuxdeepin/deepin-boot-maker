/*
 * Copyright (C) 2017 ~ 2018 Wuhan Deepin Technology Co., Ltd.
 *
 * Author:     Iceyer <me@iceyer.net>
 *
 * Maintainer: Iceyer <me@iceyer.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "widgetutil.h"

#include <QFile>
#include <QApplication>
#include <QImageReader>
#include <QIcon>

namespace WidgetUtil
{
QString getQss(const QString & qssFilename)
{
    QString qss;

    QFile themeFile(qssFilename);

    if (themeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qss = themeFile.readAll();

        themeFile.close();
    }

    return qss;
}

QPixmap getDpiPixmap(const QString filename, QWidget *w)
{
    QPixmap pixmap;
    qreal devicePixelRatio = qApp->devicePixelRatio();
    if (w) {
        devicePixelRatio = w->devicePixelRatioF();
    }

    qreal ratio = 1.0;
    if (!qFuzzyCompare(ratio, devicePixelRatio)) {
        QImageReader reader;
        reader.setFileName(qt_findAtNxFile(filename, devicePixelRatio, &ratio));
        if (reader.canRead()) {
            reader.setScaledSize(reader.size() * (devicePixelRatio / ratio));
            pixmap = QPixmap::fromImage(reader.read());
            pixmap.setDevicePixelRatio(devicePixelRatio);
        }
    } else {
        pixmap.load(filename);
    }

    return pixmap;
}
}
