#ifndef DCHECKBOX_H
#define DCHECKBOX_H

#include <QWidget>
#include <QCheckBox>
class QCheckBox;
class QLabel;
class QString;

class DCheckBox : public QWidget
{
    Q_OBJECT
public:
    explicit DCheckBox(const QString& text, QWidget *parent = 0);

    void setFixedWidth(int w);

    bool checked() {return checkState_ == Qt::Checked;}

public:
    Qt::CheckState checkState() const { return checkState_; }
    void setCheckState(Qt::CheckState state) {checkState_ = state; emit stateChanged(state);}

Q_SIGNALS:
    void stateChanged(int);

public slots:
    void click(bool checked = false);

private:
    QCheckBox       *checkBox_;
    QLabel          *label_;
    int             indicatorSize_;
    Qt::CheckState  checkState_;

};

#endif // DCHECKBOX_H
