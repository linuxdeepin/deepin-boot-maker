#include "widgetutil.h"

#include <QFile>

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
}
