#ifndef DPUSHBUTTON_H
#define DPUSHBUTTON_H

#include <QPushButton>

class DPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DPushButton(const QString& text, QWidget *parent = 0);

signals:

public slots:

};

#endif // DPUSHBUTTON_H
