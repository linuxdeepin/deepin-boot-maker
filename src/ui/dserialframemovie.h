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
    QString     m_QssTemplate;
    QStringList m_SourceList;
    int         m_FrameIndex;
    QTimer      *m_Timer;
};

#endif // DSERIALFRAMEMOVIE_H
