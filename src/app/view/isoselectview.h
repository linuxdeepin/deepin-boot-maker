// SPDX-FileCopyrightText: 2017 - 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "dropframe.h"

#include <DFrame>
#include <DLabel>
#include <DPushButton>

#include <QThread>
DWIDGET_USE_NAMESPACE

class ThreadCheckFile : public QThread
{
    Q_OBJECT
public:
    ThreadCheckFile();
    void setRestart();
    void setFile(QString file);
signals:
    void checkFileFinish(bool);
protected:
    virtual void run();

private:
    bool restart;
    QString m_file;
};

class ISOSelectView : public DWidget
{
    Q_OBJECT

    Q_PROPERTY(QString isoFilePath READ isoFilePath)
public:
    explicit ISOSelectView(DWidget *parent = nullptr);

    inline QString isoFilePath() const
    {
        return m_isoFilePath;
    }

signals:
    void requestVerfiyISOFile(const QString &file);
    void isoFileSelected(QString isoFilePath);

public slots:
    void onFileSelected(const QString &file);
    void onFileVerfiyFinished(bool ok);
    void slot_ThemeChange();

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private:
    DLabel          *m_hits         = nullptr;
    DLabel          *m_fileLabel    = nullptr;
    DLabel          *m_fileSelect   = nullptr;
    DPushButton     *m_nextSetp     = nullptr;
    DLabel          *m_title        = nullptr;
    DropFrame       *isoPanel       = nullptr;
    DLabel          *spliter        = nullptr;
    QString         m_isoFilePath;
    QString         m_selectText;
    DLabel          *growIcon       = nullptr;
    DLabel          *m_checkFile    = nullptr;
    ThreadCheckFile t_checkfile;
    DLabel          *isoIcon        = nullptr;
};

