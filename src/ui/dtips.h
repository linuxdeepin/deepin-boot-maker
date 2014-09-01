#ifndef DTIPS_H
#define DTIPS_H

#include <QPushButton>

class DTips : public QPushButton
{
    Q_OBJECT
public:
    explicit DTips(QWidget *parent = 0);

signals:
    void poped();

public slots:
    void pop();

protected slots:
    void popDone();

protected:
    QWidget *m_attachWidget;
    bool    m_active;
};

#endif // DTIPS_H
