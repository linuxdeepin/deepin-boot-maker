#include "dwaterprogress.h"

#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QtMath>

DWaterProgress::DWaterProgress(QWidget *parent) :
    QWidget(parent),
    m_shadow(new QImage(":/theme/light/image/water_shadow.png"))
{
    setFixedSize(128, 128);
    this->setMask(QRegion(0, 0, this->width(), this->height(), QRegion::Ellipse));
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateProcess()));

    m_FrontImage = new QImage(":/theme/light/image/water_front.png");
    m_BackImage = new QImage(":/theme/light/image/water_back.png");
    m_FrontWidth = m_FrontImage->width() - 1;
    m_BackWidth = m_BackImage->width() - 1;
    m_ForntXOffset1 = 0;
    m_ForntXOffset2 = -m_FrontWidth;
    m_BackXOffset1 = m_BackWidth / 2;
    m_BackXOffset2 = -m_BackWidth / 2;

    m_Text = new QLabel;

    m_Text->setStyleSheet("QLabel{color:white;font-size:20px;}");
    m_Progresss = 0;

    setProgress(m_Progresss);

    m_Pop7YOffset = 100;
    m_Pop8YOffset = 100;
    m_Pop11YOffset = 100;
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_Text);
    layout->setAlignment(m_Text, Qt::AlignCenter);
    this->setLayout(layout);
}

void DWaterProgress::setProgress(int p)
{
    int zeroY = 104;
    int fullY = 8;
    m_Progresss = (p <= 100) ? p : 100;
    m_Progresss = (m_Progresss <= 0) ? 0 : m_Progresss;
    m_YOffset = zeroY - p * (zeroY - fullY) / 100 ;
    setText(m_Progresss);
}

void DWaterProgress::setText(int p)
{
    m_Text->setText(QString("%1%").arg(p));
}

void DWaterProgress::start()
{
    m_timer->start(66);
}

void DWaterProgress::stop()
{
    m_timer->stop();
}

void DWaterProgress::updateProcess()
{
    auto wmax = 114;
    auto wmin = 12;

    m_ForntXOffset1 += 2;
    m_ForntXOffset2 += 2;
    if (m_ForntXOffset1 >= wmax) {
        m_ForntXOffset1 = -m_FrontWidth;
    }
    if (m_ForntXOffset2 >= wmax) {
        m_ForntXOffset2 = -m_FrontWidth;
    }

    m_BackXOffset1 -= 3;
    m_BackXOffset2 -= 3;
    if (m_BackXOffset1 < -m_BackWidth) {
        m_BackXOffset1 = wmax;
    }
    if (m_BackXOffset2 < -m_BackWidth) {
        m_BackXOffset2 = wmax;
    }

    if (m_YOffset < -wmin) {
        m_YOffset = 88;
    }

    m_Pop7YOffset -= 0.8;
    if (m_Pop7YOffset < m_YOffset + 10) {
        m_Pop7YOffset = 100;
    }
    m_Pop7XOffset = 35 + 14 + qSin((100 - m_Pop7YOffset) * 2 * 3.14 / 40) * 8;

    m_Pop8YOffset -= 1.2;
    if (m_Pop8YOffset < m_YOffset + 10) {
        m_Pop8YOffset = 100;
    }
    m_Pop8XOffset = 45 + 14 + qCos((100 - m_Pop8YOffset) * 2 * 3.14 / 40) * 9
                    * (100 - m_Pop8XOffset) / 60;

    m_Pop11YOffset -= 1.6;
    if (m_Pop11YOffset < m_YOffset + 10) {
        m_Pop11YOffset = 100;
    }
    m_Pop11XOffset = 55 + 14 + qSin((100 - m_Pop11YOffset) * 2 * 3.14 / 50)
                     * 11 * (100 - m_Pop11YOffset) / 60;

    this->update();
}
//#include <QDebug>
void DWaterProgress::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    auto rect = this->rect();
//    auto w = rect.width();
//    auto h = rect.height();
    auto w = 100;
    auto h = 100;

    // draw backgroud
    QPainterPath outerPath;
    outerPath.addEllipse(rect.center(), w / 2, h / 2);
    p.fillPath(outerPath, QBrush(qRgb(0xcb, 0xe0, 0xff)));

    // draw water
    p.drawImage(m_BackXOffset1, m_YOffset, *m_BackImage);
    p.drawImage(m_BackXOffset2, m_YOffset, *m_BackImage);
    p.drawImage(m_ForntXOffset1, m_YOffset, *m_FrontImage);
    p.drawImage(m_ForntXOffset2, m_YOffset, *m_FrontImage);

    //drwa pop
    QPainterPath pop7;
    pop7.addEllipse(m_Pop7XOffset, m_Pop7YOffset, 7, 7);
    p.fillPath(pop7, QColor(77, 208, 255));

    QPainterPath pop8;
    pop8.addEllipse(m_Pop8XOffset, m_Pop8YOffset, 8, 8);
    p.fillPath(pop8, QColor(77, 208, 255));

    QPainterPath pop11;
    pop11.addEllipse(m_Pop11XOffset, m_Pop11YOffset, 11, 11);
    p.fillPath(pop11, QColor(77, 208, 255));

    auto offset = 14;
    // draw boder
    QPen borderPenBk(QColor(0xff, 0xff, 0xff, 0xff), 16);
    p.setPen(borderPenBk);
    p.drawEllipse(offset - 8,  offset - 8, w + 16, h + 16);

    // draw boder
    QPen borderPen1(QColor(0xff, 0xff, 0xff, 0x80), 2);
    p.setPen(borderPen1);
    p.drawEllipse(5 + offset, 5 + offset, w - 10, h - 10);

    QPen borderPen(QColor(qRgb(43, 146, 255)), 2);
    p.setPen(borderPen);
    p.drawEllipse(3 + offset, 3 + offset, w - 6, h - 6);

    QPen smothBorderPen(Qt::white, 4);
    p.setPen(smothBorderPen);
    p.drawEllipse(offset, offset, w, h);

    p.drawImage(0, 0, *m_shadow);

}
