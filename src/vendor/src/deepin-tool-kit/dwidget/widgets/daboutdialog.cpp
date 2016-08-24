#include "daboutdialog.h"

#include <dutility.h>

#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include <QVBoxLayout>
#include <QLabel>
#include <QIcon>
#include <QKeyEvent>

DWIDGET_BEGIN_NAMESPACE

const QString DAboutDialog::websiteLinkTemplate = "<a href='%1' style='text-decoration: none;'>%2</a>";

DAboutDialog::DAboutDialog(
    const QString &windowIcon,
    const QString &productIcon,
    const QString &productName,
    const QString &version,
    const QString &description,
    QWidget *parent,
    const QString &companyLogo,
    const QString &website,
    const QString &websiteLink): DWindow(parent)
{
    setTitlebarFixedHeight(30);
    setWindowFlags(windowFlags() & ~ Qt::WindowMinimizeButtonHint);
    setWindowFlags(windowFlags() & ~ Qt::WindowSystemMenuHint);
    setWindowFlags(windowFlags() & ~ Qt::WindowMaximizeButtonHint);
    setWindowModality(Qt::ApplicationModal);

    setWindowIcon(QIcon(windowIcon));

    QLabel *logoLabel = new QLabel("logo");
    logoLabel->setContentsMargins(0, 0, 0, 0);
    logoLabel->setFixedSize(96, 96);
    logoLabel->setPixmap(QPixmap(productIcon).scaled(logoLabel->size(), Qt::KeepAspectRatio));

    QLabel *productNameLabel = new QLabel(productName);
    productNameLabel->setStyleSheet("font-size:18px;");

    QLabel *versionLabel = new QLabel(version);
    versionLabel->setStyleSheet("font-size:12px; color: #666666");

    QPixmap companyLogoPixmap(companyLogo);
    QLabel *companyLogoLabel = new QLabel(companyLogo);
    companyLogoLabel->setPixmap(companyLogoPixmap);
    companyLogoLabel->setFixedSize(companyLogoPixmap.size());

    QLabel *websiteLabel = new QLabel(website);
    websiteLabel->setFixedHeight(24);
    websiteLabel->setStyleSheet("font-size:13px; color: #004EE5");
    websiteLabel->setOpenExternalLinks(false);
    QString websiteText = QString(websiteLinkTemplate).arg(websiteLink).arg(website);
    websiteLabel->setText(websiteText);
    connect(websiteLabel, SIGNAL(linkActivated(QString)),
            this, SLOT(onLogLinkActivated(QString)));

    QLabel *descriptionLabel = new QLabel();
    descriptionLabel->setText(description + '\n');
    descriptionLabel->setAlignment(Qt::AlignHCenter);

    descriptionLabel->setStyleSheet("font-size:11px; color: #1A1A1A; border: none;");
    descriptionLabel->setWordWrap(true);
    descriptionLabel->adjustSize();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->setContentsMargins(38, 0, 38, 10);
    mainLayout->setSpacing(0);
    mainLayout->addWidget(logoLabel);
    mainLayout->setAlignment(logoLabel, Qt::AlignCenter);
    mainLayout->addSpacing(8);
    mainLayout->addWidget(productNameLabel);
    mainLayout->setAlignment(productNameLabel, Qt::AlignCenter);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(versionLabel);
    mainLayout->setAlignment(versionLabel, Qt::AlignCenter);
    mainLayout->addSpacing(12);
    mainLayout->addWidget(companyLogoLabel);
    mainLayout->setAlignment(companyLogoLabel, Qt::AlignCenter);
    mainLayout->addSpacing(2);
    mainLayout->addWidget(websiteLabel);
    mainLayout->setAlignment(websiteLabel, Qt::AlignCenter);
    mainLayout->addSpacing(26);
    mainLayout->addWidget(descriptionLabel, Qt::AlignHCenter);

    setLayout(mainLayout);

    this->setFixedWidth(400);
    this->adjustSize();
    this->setFixedSize(this->size());
    if (parent && parent->isTopLevel()) {
        QPoint pCenterGlobal = mapToGlobal(parent->geometry().center());
        this->move(pCenterGlobal.x() - width() / 2,
                   pCenterGlobal.y() - height() / 2);
    }

    setFocus();
}

DAboutDialog::DAboutDialog(
    const QString &/*windowTitle*/,
    const QString &windowIcon,
    const QString &productIcon,
    const QString &productName,
    const QString &version,
    const QString &description,
    QWidget *parent,
    const QString &companyLogo,
    const QString &website,
    const QString &websiteLink)
    : DAboutDialog(windowIcon, productIcon, productName, version, description, parent, companyLogo, website, websiteLink)
{
    setTitle("");
}

void DAboutDialog::onLogLinkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));

}

void DAboutDialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        close();

        event->accept();
    }

    DWindow::keyPressEvent(event);
}

DWIDGET_END_NAMESPACE
