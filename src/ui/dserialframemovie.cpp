#include "dserialframemovie.h"

#include <QTimer>

QString DSerialFrameMovie::s_QssTemplate =  "DSerialFrameMovie {"
        "background: url(%1);"
        "border:0px;"
        "}";

DSerialFrameMovie::DSerialFrameMovie(QWidget *parent):
    DPushButton("",parent)
{
    m_Timer=new QTimer(this);
    connect(m_Timer,SIGNAL(timeout()),this,SLOT(nextFrame()));
    m_FrameIndex = 0;

}

DSerialFrameMovie::DSerialFrameMovie(const QStringList &sourcesList, QWidget *parent):
    DPushButton("",parent)
{
    m_Timer=new QTimer(this);

    connect(m_Timer,SIGNAL(timeout()),this,SLOT(nextFrame()));
    m_SourceList = sourcesList;
    m_FrameIndex = 0;
    QString firstFrame = m_SourceList[0];
    this->setFixedSize(QPixmap(firstFrame).size());
    this->setStyleSheet(s_QssTemplate.arg(firstFrame));
}

void DSerialFrameMovie::nextFrame() {
    if (0 == m_SourceList.length()) {
        return;
    }

    if (m_FrameIndex >= m_SourceList.length()) {
        m_FrameIndex = 0;
    }
    QString source = m_SourceList[m_FrameIndex];
    this->setStyleSheet(s_QssTemplate.arg(source));
    ++m_FrameIndex;
}

void DSerialFrameMovie::start() {
    m_Timer->start(100);
}

void DSerialFrameMovie::stop() {
    if (0 == m_SourceList.length()) {
        return;
    }

    this->setStyleSheet(s_QssTemplate.arg(m_SourceList[0]));
    m_Timer->stop();
}

void DSerialFrameMovie::moveEvent(QMoveEvent *e) {
    QPushButton::moveEvent(e);
}

void DSerialFrameMovie::resizeEvent(QResizeEvent *e) {
    QPushButton::resizeEvent(e);
}

