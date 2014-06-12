#ifndef DDROPAREA_H
#define DDROPAREA_H

#include <QQuickItem>

class DDropArea : public QQuickItem
{
    Q_OBJECT

public:
    explicit DDropArea(QQuickItem *parent=0);

signals:
    void textDrop(QString text);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);


private:
    bool m_accepting;

};

#endif // DDROPAREA_H
