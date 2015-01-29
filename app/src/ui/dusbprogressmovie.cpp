#include "dusbprogressmovie.h"

#include <QLabel>
#include <QPainter>
#include <QBitmap>
#include <QStackedLayout>

#include <QTimer>

DWaterMovie::DWaterMovie(QWidget *parent) :
    QLabel(parent)
{
    m_Mask = new QImage(":/ui/images/usb/mask.png");
    m_MaskWidth = m_Mask->width();
    m_MasHeight = m_Mask->height();
    m_YOffset = 0;
    m_timer = new QTimer;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(updateProcess()));

    int begin = 8;
    int offset = 25;
    this->setProgressImage(":/ui/images/usb/wave.png", begin, begin + offset);
}

void DWaterMovie::setProgressImage(const QString &qrl, int begin, int end) {
    m_Image = new QImage(qrl);
    m_ImageWidth = m_Image->width();
    m_YMin = begin;
    m_YMax = end;
}

void DWaterMovie::start() {
    m_timer->start(33);
}

void DWaterMovie::setProgress(int p){
    m_YOffset = (m_YMax - m_YMin) * p / 100;
}


void DWaterMovie::updateProcess() {
    m_XOffset += 1;
    m_XOffset = m_XOffset % (m_ImageWidth);
    this->update();
}

void DWaterMovie::paintEvent(QPaintEvent *)
{
    if ((m_YOffset  + m_YMin) > m_YMax) {
        m_YOffset = m_YMax - m_YMin;
    }
    QPainter p(this);

    QImage mask(":/ui/images/usb/mask.png");
    int nDepth = m_Image->depth();
    int nWidth = mask.width();
    int nHeight = mask.height();
    switch ( nDepth ) {
    case 32:
        for ( int y = 0; y < nHeight; ++y ) {
            quint32* pMaskData = (quint32*)(mask.scanLine( y ));
            quint32* pWaveData = (quint32*)(m_Image->scanLine( y -nHeight + m_YMin + m_YOffset));
            quint32 alpha;
            if ((y - nHeight + m_YMin + m_YOffset) <= 0 ) {
                for ( int x = 0; x < nWidth; ++x ) {
                    pMaskData[x] = 0x00000000;
                }
            }
            for ( int x = 0; x < nWidth; ++x ) {
                alpha = (pMaskData[x] >> 24) << 24 | 0x00FFFFFF;
                pMaskData[x] = pWaveData[(x-m_XOffset)%m_ImageWidth] & (alpha);
            }
        }
        break;
    default:
        break;
    }
    p.drawPixmap(0, 0, QPixmap::fromImage(mask));
}


DUsbProgressMovie::DUsbProgressMovie(QWidget *parent) :
    QWidget(parent)
{
    QStackedLayout *m_layout = new QStackedLayout;
    m_layout->setStackingMode(QStackedLayout::StackAll);
    QLabel *usb = new QLabel;
    QPixmap pixUsb(":/ui/images/usb/usb.png");
    usb->setPixmap(pixUsb);
    m_layout->addWidget(usb);

    m_WaterProgress = new DWaterMovie;
    m_layout->addWidget(m_WaterProgress);
    m_layout->setAlignment(m_WaterProgress, Qt::AlignVCenter);

    QLabel *light = new QLabel;
    light->setMargin(1);
    light->setPixmap(QPixmap(":/ui/images/usb/light.png"));
    m_layout->addWidget(light);

    this->setLayout(m_layout);
}

void DUsbProgressMovie::start() {
    m_WaterProgress->start();
}

void DUsbProgressMovie::setProgress(int p) {
    m_WaterProgress->setProgress(p);
}
