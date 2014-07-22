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

class BootMaker;

class DWindowUI : public QMainWindow
{
    Q_OBJECT
public:
    explicit DWindowUI(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);

signals:

public slots:

private:
    void InitUI();
    void AddProcessUI(QBoxLayout * top);
    void AddOptionUI(QBoxLayout * top);

private slots:
    void start();
    void refrshUsbDriverList();

private:
    BootMaker *bootMaker_;

    QLabel * isoLabel;
    QLabel * processLabel;
    QLabel * usbLabel;

    DFileChooseInput *isoFile;
    DComboBox *usbDriver;
    DCheckBox *bisoMode;
    DCheckBox *formatDisk;

    QHBoxLayout *actionlayout;
    DPushButton *startBt;
};

#endif // DWINDOWUI_H
