#include "unmountusbview.h"
#include <bminterface.h>
UnmountUsbView::UnmountUsbView(DWidget *parent):DWidget (parent)
{
    setObjectName("safeejectview");
    setAutoFillBackground(true);
    QVBoxLayout *MainLayout=new QVBoxLayout(this);
    MainLayout->setContentsMargins(0,1,0,0);
    MainLayout->setSpacing(0);
    m_ejectIcon=new DLabel();
    m_ejectIcon->setObjectName("ejecticon");
    m_ejectIcon->setPixmap(WidgetUtil::getDpiPixmap(":/theme/dark/image/success.svg",this));
    m_hitTitle=new DLabel(tr("Ejecting the USB flash drive, please wait ..."));
    DPalette pa=DApplicationHelper::instance()->palette(m_hitTitle);
    QBrush brush=DApplicationHelper::instance()->palette(m_hitTitle).text();
    pa.setBrush(DPalette::Text,brush);
    m_hitTitle->setPalette(pa);
    m_hitTitle->setFixedWidth(400);
    m_hitTitle->setFixedHeight(25);
    DFontSizeManager::instance()->bind(m_hitTitle,DFontSizeManager::T5);
    m_hitTitle->setAlignment(Qt::AlignCenter);
    MainLayout->addSpacing(96);
    MainLayout->addWidget(m_ejectIcon,0,Qt::AlignCenter);
    MainLayout->addSpacing(0);
    MainLayout->addWidget(m_hitTitle,0,Qt::AlignCenter);
    MainLayout->addStretch();
    pa=DApplicationHelper::instance()->palette(this);//    QBrush brush2=DApplicationHelper::instance()->palette(this).base();
    pa.setBrush(DPalette::Background,pa.color(DPalette::Base));
    this->setPalette(pa);
    connect(BMInterface::instance(),&BMInterface::reportProgress1,this,
            [=](int current,quint32 error, const QString & title, const QString & description){
            emit finish(error,title,description);
    });

}
