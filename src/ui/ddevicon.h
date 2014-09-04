#ifndef DDEVICON_H
#define DDEVICON_H

#include <QWidget>
#include "dtips.h"

class DSerialFrameMovie;
class DUsbProgressMovie;

class QLabel;

class DDevIcon : public QWidget{
    Q_OBJECT
public:
    enum DevStatus {
        UnSelected,
        Selected,
        Progress,
    };

    explicit DDevIcon(QWidget *parent = 0);

    DevStatus Status() const;
    void setStatus(const DevStatus &Status);

    void setProgress(int p);

signals:
    void clicked();

protected slots:
    void emitClicked();

protected:

    DSerialFrameMovie   *m_BreatheMovie;
    DUsbProgressMovie   *m_ProgressMovie;
    QWidget             *m_ProgressWidget;
    QWidget             *m_LightWidget;
    DevStatus           m_Status;
};

#endif // DDEVICON_H
