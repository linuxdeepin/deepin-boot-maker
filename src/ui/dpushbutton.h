#ifndef DPUSHBUTTON_H
#define DPUSHBUTTON_H

#include <QPushButton>
class DTips;

class DPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit DPushButton(const QString& text, QWidget *parent = 0);

    void setImages(const QString& normal, const QString& hover, const QString& pressed);
    void setDTips(const QString& tips) {m_tipsString = tips;}
    void showDTips(bool show) {m_showDtips = show;}
    void ingonreLeaveEvent() {m_ingonreLeaveEvent = true;}
signals:
    void entered();
    void leaved();


protected:
    virtual void enterEvent( QEvent* e );
    virtual void leaveEvent(QEvent * e);

protected slots:
    void dtipPoped();
    void leaveDone();

protected:
    QString m_tipsString;
    DTips   *m_dtips;
    bool    m_showDtips;
    bool    m_dtipsPoping;
    bool    m_mouseInRect;
    bool    m_ingonreLeaveEvent;
};

#endif // DPUSHBUTTON_H
