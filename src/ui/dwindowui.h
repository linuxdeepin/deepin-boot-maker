#ifndef DWINDOWUI_H
#define DWINDOWUI_H

#include <QMainWindow>

class DWindowUI : public QMainWindow
{
    Q_OBJECT
public:
    explicit DWindowUI(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);

signals:

public slots:

private:
    void InitUI();

};

#endif // DWINDOWUI_H
