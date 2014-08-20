#ifndef DWINDOWUI_H
#define DWINDOWUI_H

#include <QMainWindow>

class QBoxLayout;
class QLabel;
class DCheckBox;
class DComboBox;
class DCloseButton;
class DFileChooseInput;
class DPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QTimer;
class BootMaker;
class DRoteMovie;
class QPushButton;
class DProgressFrame;

class DWindowUI : public QMainWindow
{
    Q_OBJECT
public:
    explicit DWindowUI(QWidget *parent = 0);
    ~DWindowUI();

    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void refrshUsbDrivers(const QStringList&);

public slots:
    void disableStartButton();
    void enableStartButton();

private:
    void InitUI();
    void AddProcessUI(QBoxLayout * top);
    void AddOptionUI(QBoxLayout * top);

    void SwitchToProcessUI();

private slots:
    void SwitchToEndUI();
    void start();
    void refrshUsbDriverList();
    void checkProcess();
    void fileSelect(const QString& filePath);
    void confirmFormat();

private:
    QPoint      pos_;
    bool        pressed_;

    BootMaker   *bootMaker_;

    QVBoxLayout     *m_topLayout;
    DCloseButton    *m_closeButton;
    DRoteMovie  *isoLabel_;
    QLabel          *processLabel_;
    QLabel          *m_progressText;
    QLabel          *m_processHints;

    DComboBox       *usbDriver_;
    DCheckBox       *bisoMode_;
    DCheckBox       *m_formatCheckBox;

    QVBoxLayout     *selectLayout_;

    QHBoxLayout     *actionLayout_;
    DPushButton     *m_start;

    QTimer          *m_usbRefreshTimer;
    QTimer          *processTimer_;

    DProgressFrame  *m_progressFrame;
    QVBoxLayout     *m_progressLayout;
    QVBoxLayout     *m_optionLayout;
};

#endif // DWINDOWUI_H
