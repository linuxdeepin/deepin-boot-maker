#ifndef PROGRESSVIEW_H
#define PROGRESSVIEW_H

#include <QWidget>

class ProgressView : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressView(QWidget *parent = 0);

signals:
    void finish(quint32 error, const QString &title, const QString &description);
    void testCancel();

};

#endif // PROGRESSVIEW_H
