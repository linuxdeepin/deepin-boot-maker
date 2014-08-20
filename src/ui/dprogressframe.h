#ifndef DPROGRESSFRAME_H
#define DPROGRESSFRAME_H

#include <QStackedWidget>

class DRoteMovie;
class QLabel;
class DPushButton;
class DTips;
class DUSBList;
class DSerialFrameMovie;
class QParallelAnimationGroup;
class DDigitProgressMovie;
class QVBoxLayout;
class QMovie;
class DUsbProgressMovie;

class DProgressFrame : public QStackedWidget
{
    Q_OBJECT
public:
    explicit DProgressFrame(QWidget *parent = 0);

    QString isoFile() const;
    void setIsoFile(const QString &isoFile);

    QString usbDev() const;
    void setUsbDev(const QString &usbDev);

    void setProgress(int p);
signals:
    void isoFileSelected(const QString&);
    void changedUsbSeclet();
    void changedProgress();

public slots:
    void toggleShadow();
    void switchShowStatus();

    void slideUsbSeclect();
    void slideProcess();

    void usbDevSelected(const QString&);
    void finishSelectDev(const QString&);

    void switchProgress();

protected slots:
    void slideUsbDone();
    void slideProgressDone();

    void selectISO();
    void refreshUsbDrivers(const QStringList&);


protected:
    enum ShowStatus{
        ShowFirst,
        ShowSecond,
    };

    bool                    m_Active;
    ShowStatus              m_showStatus;
    QWidget                 *m_topShadow;
    QWidget                 *m_bottonShadow;
    QWidget                 *m_bottonShadowFrame;
    QWidget                 *m_firstWidget;
    QWidget                 *m_secondWidget;
    QVBoxLayout             *firstLayout;
    QLabel                  *m_processLabel;
    //DDigitProgressAnimation *m_processLabel;
    DUsbProgressMovie   *m_processUsb;
    DTips                   *m_usbTpis;
    DTips                   *tipsIso;
    DSerialFrameMovie   *m_isoLabel;
    DSerialFrameMovie   *m_usbLabel;
    QParallelAnimationGroup *m_animgroup;
    DUSBList *              m_listView;
    int                     m_speed;

    QString                 m_isoFile;
    QString                 m_usbDev;


public slots:

};

#endif // DPROGRESSFRAME_H
