#include "dwindowui.h"

#include <QPainter>
#include <QLayout>
#include <QLabel>
#include <QAnimationGroup>

DWindowUI::DWindowUI(QWidget *parent) :
    QMainWindow(parent)
{
    resize(680, 440);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    QString style = "DWindowUI {"
            "margin : 1px"
            "}";
    setStyleSheet(style);

    InitUI();
}

void DWindowUI::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.save();
    p.drawPixmap(0,0, QPixmap(":/ui/images/uibackgound.png"));
    p.restore();
}

void DWindowUI::InitUI() {
    QHBoxLayout *top = new QHBoxLayout();
    QVBoxLayout *process = new QVBoxLayout();
    //QImage iso(":/image/iso-inactive.png");
    QLabel *isolabel =new QLabel();
    isolabel->setPixmap(QPixmap(":/image/iso-inactive.png"));
    process->addWidget(isolabel);

    top->addLayout(process);

    QWidget *window = new QWidget();
    window->setLayout(top);
    setCentralWidget(window);
}
