#ifndef DIMAGEICON_H
#define DIMAGEICON_H

#include "dserialframemovie.h"
#include "dtips.h"
#include <QWidget>

class DSerialFrameMovie;

class DImageIcon : public DSerialFrameMovie
{
    Q_OBJECT
public:
    enum ImageStatus {
        UnSelected,
        Selected,
        Progress,
    };


    explicit DImageIcon(QWidget *parent = 0);

    void setStatus(ImageStatus status) {
        m_Status = status;
        emit statusChanged(m_Status);
    }

    ImageStatus status() {
        return m_Status;
    }

signals:
    void statusChanged(ImageStatus);

public slots:
    void updateStatus(ImageStatus);

protected:
    ImageStatus         m_Status;
    QStringList         m_BreatheMovieSources;
    QStringList         m_RoateMovieSources;
};

#endif // DIMAGEICON_H
