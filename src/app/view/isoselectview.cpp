#include "isoselectview.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QFrame>

ISOSelectView::ISOSelectView(QWidget *parent) : QFrame(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 9, 0, 0);

    QLabel *m_title = new QLabel(tr("请选择光盘文件"));
    m_title->setFixedHeight(38);
    m_title->setStyleSheet("font-size: 26px;");

    QLabel *isoIcon = new QLabel;
    isoIcon->setPixmap(QPixmap(":/media-optical-96px.png"));

    QFrame *isoPanel = new QFrame;
    isoPanel->setFixedSize(410, 320);
    QVBoxLayout *isoPanelLayout = new QVBoxLayout(isoPanel);

    isoPanelLayout->addWidget(isoIcon, 0, Qt::AlignCenter);


    mainLayout->addWidget(m_title, 0, Qt::AlignCenter);
    mainLayout->addSpacing(31);
    mainLayout->addWidget(isoPanel, 0, Qt::AlignCenter);
    mainLayout->addStretch();
}
