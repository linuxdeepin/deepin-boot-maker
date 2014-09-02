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
    int     m_Margin;
    int     m_Radius;
    QPoint  m_LastMousePos;
    bool    m_MousePressed;

    BootMaker        *m_BootMaker;

    QVBoxLayout     *m_topLayout;
    QVBoxLayout     *m_progressLayout;
    QVBoxLayout     *m_optionLayout;
    DCloseButton    *m_closeButton;
    QLabel          *m_progressText;
    QLabel          *m_processHints;
    DCheckBox       *m_formatCheckBox;
    QHBoxLayout     *m_ActionLayout;
    DPushButton     *m_start;
    DProgressFrame  *m_progressFrame;
    QTimer          *m_usbRefreshTimer;
    QTimer          *m_ProcessTimer;
};

#endif // DWINDOWUI_H
