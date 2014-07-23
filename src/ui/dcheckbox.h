#ifndef DCHECKBOX_H
#define DCHECKBOX_H

#include <QWidget>
#include <QCheckBox>
class QCheckBox;
class QLabel;
class QString;
class QPushButton;

class DCheckBox : public QWidget
{
    Q_OBJECT
public:
    explicit DCheckBox(const QString& text, QWidget *parent = 0);

    void setFixedWidth(int w);

    void setChecked(bool checked);
    bool checked() {
        return checkState_ == Qt::Checked;
    }

public:
    Qt::CheckState checkState() const {
        return checkState_;
    }
    void setCheckState(Qt::CheckState state) {
        checkState_ = state;
        emit stateChanged(state);
    }

Q_SIGNALS:
    void stateChanged(int);
    void clicked();

public slots:
    void click();

private:
    QPushButton     *checkBox_;
    QLabel          *label_;
    int             indicatorSize_;
    Qt::CheckState  checkState_;

    QString         styleChecked_;
    QString         styleUnchecked_;

};

#endif // DCHECKBOX_H
