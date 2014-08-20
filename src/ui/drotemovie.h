#ifndef QROTEANIMATION_H
#define QROTEANIMATION_H

#include <QWidget>
#include <QPixmap>

class QTimer;

class DRoteMovie : public QWidget
{
    Q_OBJECT
public:
    explicit DRoteMovie(const QPixmap &pixmap, QWidget *parent = 0);

    void paintEvent(QPaintEvent * /*event*/);

    void start();

    void stop();

    void setPixmap(const QPixmap &pixmap);

signals:

public slots:
    void refresh();

private:
    QTimer  *timer_;
    QPixmap pixmap_;
    qreal   degrees_;
};

#endif // QROTEANIMATION_H
