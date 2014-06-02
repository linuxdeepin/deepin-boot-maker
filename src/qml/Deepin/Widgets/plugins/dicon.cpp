#include <QIcon>
#include <QDir>
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QPainter>

#include "dicon.h"

DIcon::DIcon(QQuickPaintedItem *parent)
    : QQuickPaintedItem(parent)
{
    QString homeDir = QDir::homePath();
    QStringList searchPaths;
    searchPaths.append(homeDir + "/.icons");
    searchPaths.append(homeDir + "/.local/share/icons");
    searchPaths.append("/usr/local/share/icons");
    searchPaths.append("/usr/share/icons");
    searchPaths.append(":/icons");
    QIcon::setThemeSearchPaths(searchPaths);
}

DIcon::~DIcon()
{

}


void DIcon::setTheme(const QString &v)
{
    m_theme = v;
    Q_EMIT themeChanged(v);
    QRect rect = QRect(0, 0, this->width(), this->height());
    this->update(rect);
}

void DIcon::setIcon(const QString &v)
{
    m_icon = v;
    Q_EMIT iconChanged(v);
    QRect rect = QRect(0, 0, this->width(), this->height());
    this->update(rect);
}

void DIcon::paint(QPainter *painter)
{
    QIcon::setThemeName(m_theme);
    QIcon icon = QIcon::fromTheme(m_icon);

    if(QFile::exists(m_icon)){
        icon = QIcon(m_icon);
    }

    QPixmap pixmap = icon.pixmap(this->width(), this->height());
    if(pixmap.isNull()){
        icon = QIcon::fromTheme("application-default-icon");
        pixmap = icon.pixmap(this->width(), this->height());
    }

    QRect rect = QRect(0, 0, this->width(), this->height());
    painter->drawPixmap(rect, pixmap, rect);
}
