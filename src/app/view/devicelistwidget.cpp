#include "devicelistwidget.h"

#include "widgetutil.h"

DeviceListWidget::DeviceListWidget(QWidget *parent) : QListWidget(parent)
{
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFocusPolicy(Qt::NoFocus);
    setFlow(QListView::TopToBottom);
    setMovement(QListView::Static);
    setViewMode(QListView::ListMode);

    this->setStyleSheet(WidgetUtil::getQss(":/theme/light/DeviceListWidget.theme"));
}
