#ifndef DSERIALFRAMEMOVIE_H
#define DSERIALFRAMEMOVIE_H

#include "dpushbutton.h"
#include <QStringList>

class QTimer;

class DSerialFrameMovie : public DPushButton
{
    Q_OBJECT
public:
    explicit DSerialFrameMovie(const QStringList& sourcesList, QWidget *parent = 0);

    void start();
    void stop();

    virtual void moveEvent(QMoveEvent *);
    virtual void resizeEvent(QResizeEvent *);
signals:

public slots:
    void nextFrame();

protected:
    QString     m_qssTemplate;
    QStringList m_sourceList;
    int         m_frameIndex;
    QTimer      *m_timer;
};

#endif // DSERIALFRAMEMOVIE_H
