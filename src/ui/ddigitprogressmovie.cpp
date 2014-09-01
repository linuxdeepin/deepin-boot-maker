#include "ddigitprogressmovie.h"

#include <QPainter>
#include <QBoxLayout>

DDigitProgressMovie::DDigitProgressMovie(QWidget *parent) :
    QWidget(parent)
{
    this->setStyleSheet("DDigitProgressAnimation { border: 1px solid gray; }");
}

void DDigitProgressMovie::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.save();
    QRectF target(10.0, 20.0, 80.0, 60.0);
    QRectF source(0.0, 0.0, 70.0, 40.0);
    QPixmap pixmap(":/ui/images/usb/1.png");
    p.drawPixmap(0, 0, pixmap);
    p.drawPixmap(target, pixmap, source);
    p.restore();
}
