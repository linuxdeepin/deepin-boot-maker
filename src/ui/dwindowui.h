#ifndef DWINDOWUI_H
#define DWINDOWUI_H

#include <QMainWindow>

class QBoxLayout;
class QLabel;
class DCheckBox;
class DComboBox;
class DFileChooseInput;
class DPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QTimer;
class BootMaker;
class DRoteAnimation;

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

public slots:

private:
    void InitUI();
    void AddProcessUI(QBoxLayout * top);
    void AddOptionUI(QBoxLayout * top);

    void SwitchToProcessUI();
    void SwitchToEndUI();

private slots:
    void start();
    void refrshUsbDriverList();
    void checkProcess();
    void fileSelect(const QString& filePath);
    void confirmFormat();

private:
    QPoint      pos_;
    bool        pressed_;

    BootMaker   *bootMaker_;

    DRoteAnimation  *isoLabel_;
    QLabel          *processLabel_;
    QLabel          *usbLabel_;

    DFileChooseInput    *isoFile_;
    DComboBox       *usbDriver_;
    DCheckBox       *bisoMode_;
    DCheckBox       *formatDisk_;

    QVBoxLayout     *selectLayout_;

    QHBoxLayout     *actionLayout_;
    DPushButton     *startBt_;

    QTimer          *usbTimer_;
    QTimer          *processTimer_;
};

#endif // DWINDOWUI_H
