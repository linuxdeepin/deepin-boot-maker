#ifndef ISOSELECTVIEW_H
#define ISOSELECTVIEW_H

#include <QFrame>

class QLabel;

class ISOSelectView : public QFrame
{
    Q_OBJECT
public:
    explicit ISOSelectView(QWidget *parent = 0);

signals:

public slots:
    void onFileSelected(const QString &file);

private:

    QLabel  *m_fileLabel;
    QLabel  *m_hits;
    QLabel  *m_fileSelectButton;

};

#endif // ISOSELECTVIEW_H
