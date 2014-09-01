#include "dtips.h"
#include <QPropertyAnimation>
#include <QSizePolicy>

DTips::DTips(QWidget *parent):
    QPushButton(parent)
{
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
    this->setStyleSheet(qss);
    this->setFocusPolicy(Qt::NoFocus);
    this->setFixedHeight(this->font().pointSize() + 24);
    this->setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding,QSizePolicy::MinimumExpanding));

    m_attachWidget = parent;
    QWidget *parentWidget = qobject_cast<QWidget*>(parent->parent());
    if (parentWidget) {
        this->setParent(parentWidget);
    }
    m_active = false;
}

void DTips::pop() {
    if (m_active){
        return;
    }
    m_active = true;
    this->show();
    this->raise();
    this->raise();
    QPropertyAnimation *movie = new QPropertyAnimation(this, "geometry");
    movie->setDuration(300);
    movie->setEasingCurve(QEasingCurve::InOutCubic);
    QPoint pos = m_attachWidget->pos();
    QSize szLabel = m_attachWidget->size();

    QSize sz = this->size();
    movie->setStartValue(QRect(pos.x() + szLabel.width() / 2 , pos.y() - szLabel.height()/2 + 5, 0, 0));
    movie->setEndValue(QRect(pos.x() + szLabel.width() / 2 - sz.width()/2,
                                 pos.y() - sz.height() + 5,
                                 sz.width(),
                                 sz.height()));
    movie->start();
    connect(movie, SIGNAL(finished()), this, SLOT(popDone()));
}

void DTips::popDone(){
    m_active = false;
    emit poped();
}
