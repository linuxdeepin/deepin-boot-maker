#ifndef DWATERPROGRESS_H
#define DWATERPROGRESS_H

#include <QWidget>
#include <memory>

class QTimer;
class QImage;
class QLabel;

class DWaterProgress : public QWidget
{
    Q_OBJECT
public:
    explicit DWaterProgress(QWidget *parent = 0);

    virtual void paintEvent(QPaintEvent *);

signals:

public slots:
    void start();
    void stop();
    void setProgress(int);

private slots:
    void updateProcess();

private:
    void setText(int);

    int m_ForntXOffset1;
    int m_ForntXOffset2;
    int m_BackXOffset1;
    int m_BackXOffset2;
    int m_YOffset;
    int m_FrontWidth;
    int m_BackWidth;

    double m_Pop7YOffset;
    double m_Pop7XOffset;

    double m_Pop8YOffset;
    double m_Pop8XOffset;

    double m_Pop11YOffset;
    double m_Pop11XOffset;

    std::unique_ptr<QImage> m_shadow;

    QImage *m_BackImage;
    QImage *m_FrontImage;
    QTimer *m_timer;
    QLabel *m_Text;
    int     m_Progresss;
};

#endif // DWATERPROGRESS_H
