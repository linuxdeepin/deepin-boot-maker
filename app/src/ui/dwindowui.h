/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

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

    static DWindowUI* CurrentWindow() {return s_MainWindow;}

    void paintEvent(QPaintEvent *);

    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void refrshUsbDrivers(const QStringList&);

public slots:
    void disableStartButton();
    void enableStartButton();
    void disableFormatOption(bool show);

private:
    void initUI();
    void addProcessUI(QBoxLayout * top);
    void addOptionUI(QBoxLayout * top);

signals:
    void confirmFormatPrompt();

private slots:
    void start();
    void refrshUsbDriverList(const QStringList &list);
    void checkProcess();
    void confirmFormat();
    void popConfirmFormatPrompt();
    void switchToProcessUI();
    void switchToEndUI();
    void switchToErrorUI();
    void onLogLinkActivated(const QString &link);

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
    QTimer          *m_ProcessTimer;

    QWidget         *m_selectWidget;
    QWidget         *m_processWidget;
    QWidget         *m_finishWidget;
    QWidget         *m_errorWidget;

    static DWindowUI* s_MainWindow;
};

#endif // DWINDOWUI_H
