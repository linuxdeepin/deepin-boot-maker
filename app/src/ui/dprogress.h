#ifndef DPROGRESS_H
#define DPROGRESS_H

#include <QWidget>
#include "dserialframemovie.h"

class DProgress : public DSerialFrameMovie
{
    Q_OBJECT
public:
    enum ProcessStatus {
        Empty,
        UnProgress,
        Progress,
    };

    explicit DProgress(QWidget *parent = 0);

    void setStatus(ProcessStatus status) {
        m_Status = status;
        emit statusChanged(m_Status);
    }

    ProcessStatus status() {
        return m_Status;
    }

signals:
    void statusChanged(ProcessStatus);

public slots:
    void updateStatus(ProcessStatus);

protected:
    ProcessStatus         m_Status;
    QStringList           m_emptySources;
    QStringList           m_inactiveSources;
    QStringList           m_activeSources;

};

#endif // DPROGRESS_H
