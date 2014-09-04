#ifndef DTIPS_H
#define DTIPS_H

#include <QLabel>

class DTips : public QLabel
{
    Q_OBJECT
public:
    explicit DTips(QWidget *parent = 0);

signals:
    void poped();

public slots:
    void pop();
    void pack();

protected slots:
    void popDone();

protected:
    bool    m_active;
    QWidget *m_AttachWidget;
};

#endif // DTIPS_H
