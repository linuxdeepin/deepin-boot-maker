#include "dserialframemovie.h"

#include <QTimer>

DSerialFrameMovie::DSerialFrameMovie(const QStringList &sourcesList, QWidget *parent):
    DPushButton("",parent)
{
    m_Timer=new QTimer(this);

    connect(m_Timer,SIGNAL(timeout()),this,SLOT(nextFrame()));
    m_SourceList = sourcesList;
    m_FrameIndex = 0;
    m_QssTemplate =  "DSerialFrameMovie {"
            "background: url(%1);"
            "border:0px;"
            "}";
    QString firstFrame = m_SourceList[0];
    this->setFixedSize(QPixmap(firstFrame).size());
    this->setStyleSheet(m_QssTemplate.arg(firstFrame));
}

void DSerialFrameMovie::nextFrame() {
    if (m_FrameIndex >= m_SourceList.length()) {
        m_FrameIndex = 0;
    }
    QString source = m_SourceList[m_FrameIndex];
    this->setStyleSheet(m_QssTemplate.arg(source));
    ++m_FrameIndex;
}

void DSerialFrameMovie::start() {
    m_Timer->start(100);
}

void DSerialFrameMovie::stop() {
    this->setStyleSheet(m_QssTemplate.arg(m_SourceList[0]));
    m_Timer->stop();
}

void DSerialFrameMovie::moveEvent(QMoveEvent *e) {
    QPushButton::moveEvent(e);
}

void DSerialFrameMovie::resizeEvent(QResizeEvent *e) {
    QPushButton::resizeEvent(e);
}

