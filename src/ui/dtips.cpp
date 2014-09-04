#include "dtips.h"
#include <QPropertyAnimation>
#include <QSizePolicy>
#include "dwindowui.h"
DTips::DTips(QWidget *parent):
    QLabel(parent)
{
    m_AttachWidget = parent;
    this->setParent(DWindowUI::CurrentWindow());
    QString qss =
        "DTips { "
        "color:#ebab4c;"
        "font-size: 10px;"
        "border-image : url(:/ui/images/tips.png) 5 6 17 6;"
        "border-top: 5px transparent;"
        "border-bottom: 17px transparent;"
        "border-right: 6px transparent;"
        "border-left: 6px transparent;"
        "}";
    this->setAlignment(Qt::AlignCenter);
    this->setStyleSheet(qss);
    this->setFocusPolicy(Qt::NoFocus);
    this->setFixedHeight(this->font().pointSize() + 24);
    this->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding));
    this->setWindowFlags(Qt::WindowStaysOnTopHint );
    m_active = false;
    this->setFrameStyle(Qt::FramelessWindowHint);
}

void DTips::pack() {
    this->hide();
}

void DTips::pop() {
    if (m_active){
        return;
    }
    m_active = true;
    this->show();
    QPropertyAnimation *movie = new QPropertyAnimation(this, "geometry");
    movie->setDuration(300);
    movie->setEasingCurve(QEasingCurve::InOutCubic);
    QPoint pos = m_AttachWidget->mapToGlobal(m_AttachWidget->pos()) - m_AttachWidget->pos();
    QPoint attachPos = this->mapFromGlobal(pos);
    QSize szLabel = m_AttachWidget->size();

    QSize sz = this->size();
    movie->setStartValue(QRect(attachPos.x() + szLabel.width() / 2 , attachPos.y() - szLabel.height()/2 + 5, 0, 0));
    movie->setEndValue(QRect(attachPos.x() + szLabel.width() / 2 - sz.width()/2,
                                 attachPos.y() - sz.height() + 5,
                                 sz.width(),
                                 sz.height()));
    movie->start();
    connect(movie, SIGNAL(finished()), this, SLOT(popDone()));
}

void DTips::popDone(){
    m_active = false;
    emit poped();
}
