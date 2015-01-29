#include "drotemovie.h"
#include <QPixmap>
#include <QTransform>
#include <QTimer>
#include <QPainter>

DRoteMovie::DRoteMovie(const QPixmap &pixmap, QWidget *parent) :
    QWidget(parent)
{
    pixmap_ = pixmap;
    setFixedSize(pixmap_.size());

    timer_ = new QTimer(this);
    timer_->setInterval(25);
    connect(timer_, SIGNAL(timeout()), this, SLOT(refresh()));
}

void DRoteMovie::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setRenderHint(QPainter::Antialiasing);
    QPointF center(pixmap_.width() / qreal(2), pixmap_.height() / qreal(2));
    painter.translate(center);
    painter.rotate(degrees_);
    painter.translate(-center);
    painter.drawPixmap(QPointF(0, 0), pixmap_);
}

void DRoteMovie::refresh(){
    degrees_ += 5;
    update();
}

void DRoteMovie::start() {
    timer_->start();
}

void DRoteMovie::stop() {
    timer_->stop();
}

void DRoteMovie::setPixmap(const QPixmap &pixmap) {
    pixmap_ = pixmap;
    update();
}
