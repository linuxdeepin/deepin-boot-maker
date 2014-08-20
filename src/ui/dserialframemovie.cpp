#include "dserialframemovie.h"

#include <QTimer>

DSerialFrameMovie::DSerialFrameMovie(const QStringList &sourcesList, QWidget *parent):
    DPushButton("",parent)
{
    m_timer=new QTimer(this);

    connect(m_timer,SIGNAL(timeout()),this,SLOT(nextFrame()));
    m_sourceList = sourcesList;
    m_frameIndex = 0;
    m_qssTemplate =  "DSerialFrameMovie {"
            "background: url(%1);"
            "border:0px;"
            "}";
    QString firstFrame = m_sourceList[0];
    this->setFixedSize(QPixmap(firstFrame).size());
    this->setStyleSheet(m_qssTemplate.arg(firstFrame));
}

void DSerialFrameMovie::nextFrame() {
    if (m_frameIndex >= m_sourceList.length()) {
        m_frameIndex = 0;
    }
    QString source = m_sourceList[m_frameIndex];
    this->setStyleSheet(m_qssTemplate.arg(source));
    ++m_frameIndex;
}

void DSerialFrameMovie::start() {
    m_timer->start(100);
}

void DSerialFrameMovie::stop() {
    m_timer->stop();
}

void DSerialFrameMovie::moveEvent(QMoveEvent *e) {
    QPushButton::moveEvent(e);
}

void DSerialFrameMovie::resizeEvent(QResizeEvent *e) {
    QPushButton::resizeEvent(e);
}

