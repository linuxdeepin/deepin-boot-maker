#pragma once

#include <DWindow>

DWIDGET_BEGIN_NAMESPACE

class DAboutDialog : public DWindow
{
    Q_OBJECT
public:
    DAboutDialog(
        const QString &windowIcon,
        const QString &productIcon,
        const QString &productName,
        const QString &version,
        const QString &description,
        QWidget *parent = nullptr,
        const QString &companyLogo = ":/images/deepin-logo.png",
        const QString &website = "www.deepin.org",
        const QString &websiteLink = "https://www.deepin.org");

    DAboutDialog(
        const QString &windowTitle,
        const QString &windowIcon,
        const QString &productIcon,
        const QString &productName,
        const QString &version,
        const QString &description,
        QWidget *parent = nullptr,
        const QString &companyLogo = ":/images/deepin-logo.png",
        const QString &website = "www.deepin.org",
        const QString &websiteLink = "https://www.deepin.org");


public slots:
    void onLogLinkActivated(const QString &link);

protected:
    void keyPressEvent(QKeyEvent * event);

private:
    static const QString websiteLinkTemplate;

};

DWIDGET_END_NAMESPACE
