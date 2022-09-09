// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#include "widgetutil.h"

#include <DApplication>

#include <QFile>
#include <QImageReader>
#include <QIcon>

namespace WidgetUtil {
QString getQss(const QString &qssFilename)
{
    QString qss;

    QFile themeFile(qssFilename);

    if (themeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qss = themeFile.readAll();

        themeFile.close();
    }

    return qss;
}

QPixmap getDpiPixmap(const QString filename, DWidget *w)
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
