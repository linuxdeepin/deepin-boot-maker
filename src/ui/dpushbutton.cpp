#include "dpushbutton.h"
#include "dtips.h"

DPushButton::DPushButton(const QString& text, QWidget *parent) :
    QPushButton(text, parent)
{
    QString style = "DPushButton{"
        "color: #b4b4b4;"
        "font-size: 12px;"
        "border-image:url(:/ui/images/transparent_button_normal.png) 3 6 3 6;"
        "border-top: 3px transparent;"
        "border-bottom: 3px transparent;"
        "border-right: 6px transparent;"
        "border-left: 6px transparent;"
    "}"
    "DPushButton:hover{"
        "border-image:url(:/ui/images/transparent_button_hover.png) 3 6 3 6;"
        "border-top: 3px transparent;"
        "border-bottom: 3px transparent;"
        "border-right: 6px transparent;"
        "border-left: 6px transparent;"
    "}"
    "DPushButton:pressed{"
        "border-image:url(:/ui/images/transparent_button_press.png) 3 6 3 6;"
        "border-top: 3px transparent;"
        "border-bottom: 3px transparent;"
        "border-right: 6px transparent;"
        "border-left: 6px transparent;"
    "}";
    setFocusPolicy(Qt::NoFocus);
    setStyleSheet(style);
    m_showDtips = false;
    m_dtipsPoping = false;
    m_mouseInRect = false;
    m_ingonreLeaveEvent = false;
    m_dtips= NULL;
    m_tipsString = "";
    connect(this, SIGNAL(leaved()), this, SLOT(leaveDone()));
}

void DPushButton::setImages(const QString &normal, const QString &hover, const QString &pressed){
    QString style = "QPushButton{"
        "color: #b4b4b4;"
        "font-size: 12px;"
        "border-image:url(%1) 3 6 3 6;"
        "border-top: 3px transparent;"
        "border-bottom: 3px transparent;"
        "border-right: 6px transparent;"
        "border-left: 6px transparent;"
    "}"
    "QPushButton:hover{"
        "border-image:url(%2) 3 6 3 6;"
        "border-top: 3px transparent;"
        "border-bottom: 3px transparent;"
        "border-right: 6px transparent;"
        "border-left: 6px transparent;"
    "}"
    "QPushButton:pressed{"
        "border-image:url(%3) 3 6 3 6;"
        "border-top: 3px transparent;"
        "border-bottom: 3px transparent;"
        "border-right: 6px transparent;"
        "border-left: 6px transparent;"
    "}";
    style = style.arg(normal).arg(hover).arg(pressed);
    setStyleSheet(style);
}


void DPushButton::enterEvent( QEvent* e ) {
    m_mouseInRect = true;
    if (m_showDtips && !m_tipsString.isEmpty()) {
        if (!m_dtips && !m_dtipsPoping) {
            m_dtips = new DTips(this);
            m_dtips->setText(m_tipsString);
            m_dtips->pop();
            m_dtipsPoping = true;
            connect(m_dtips, SIGNAL(poped()), this, SLOT(dtipPoped()));
        }
    }
    emit entered();
    QWidget::enterEvent(e);
}

void DPushButton::dtipPoped() {
    if (!m_mouseInRect) {
        m_dtips->deleteLater();
        m_dtips = NULL;
    }
    m_dtipsPoping = false;
}

void DPushButton::leaveDone(){
    if (!m_dtipsPoping && m_dtips){
        m_dtipsPoping = false;
        m_dtips->deleteLater();
        m_dtips = NULL;
    }
}

void DPushButton::leaveEvent( QEvent* e ) {
    m_mouseInRect = false;
    emit leaved();
    QWidget::leaveEvent(e);
}
