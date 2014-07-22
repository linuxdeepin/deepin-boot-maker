#ifndef DFILECHOOSEINPUT_H
#define DFILECHOOSEINPUT_H

#include <QWidget>

class QLineEdit;
class QPushButton;

class DFileChooseInput : public QWidget
{
    Q_OBJECT
public:
    explicit DFileChooseInput(QWidget *parent = 0);

    QString text() {return text_;}

    void setFixedWidth(int w);
    void setFixedHeight(int h);

signals:

public slots:
    void selectFile();

private:
    int         btWidth_;
    QString     text_;
    QLineEdit   *edit_;
    QPushButton *bt_;
};

#endif // DFILECHOOSEINPUT_H
