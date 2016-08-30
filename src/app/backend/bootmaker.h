#ifndef BOOTMAKER_H
#define BOOTMAKER_H

#include <QObject>

class BMReporter;
class BootMaker : public QObject
{
    Q_OBJECT
public:
    explicit BootMaker(QObject *parent = 0);

signals:
    void finished(int errcode, const QString &description);
//    void progressChanged(int current, int total);
    void reportProgress(int current, int total, const QString &title, const QString &description);

public slots:
    bool install(const QString &image,
                 const QString &device,
                 const QString &partition,
                 bool  formatDevice);


//    int blobsInstall();
//    int diskFormat();
//    int isoExtract();
//    int bootloaderInstall();
private:

    BMReporter *m_bmr = nullptr;

};

#endif // BOOTMAKER_H
