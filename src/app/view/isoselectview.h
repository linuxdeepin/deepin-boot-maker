#ifndef ISOSELECTVIEW_H
#define ISOSELECTVIEW_H

#include <QWidget>

class QLabel;
class SuggestButton;

class ISOSelectView : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString isoFilePath READ isoFilePath)
public:
    explicit ISOSelectView(QWidget *parent = 0);

    inline QString isoFilePath() const
    {
        return m_isoFilePath;
    }

signals:
    void requestVerfiyISOFile(const QString &file);
    void isoFileSelected();

public slots:
    void onFileSelected(const QString &file);
    void onFileVerfiyFinished(bool ok);

private:
    QLabel          *m_hits         = nullptr;
    QLabel          *m_fileLabel    = nullptr;
    QLabel          *m_fileSelect   = nullptr;
    SuggestButton   *m_nextSetp     = nullptr;
    QString         m_isoFilePath;

};

#endif // ISOSELECTVIEW_H
