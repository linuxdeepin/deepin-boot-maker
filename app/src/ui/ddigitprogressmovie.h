#ifndef DDIGITPROGRESSMOVIE_H
#define DDIGITPROGRESSMOVIE_H

#include <QLabel>

class DDigitProgressMovie : public QWidget
{
    Q_OBJECT
public:
    explicit DDigitProgressMovie(QWidget *parent = 0);

    virtual void paintEvent(QPaintEvent *);

signals:

public slots:

};

#endif // DDIGITPROGRESSMOVIE_H
