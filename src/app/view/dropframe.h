#ifndef DROPFRAME_H
#define DROPFRAME_H

#include <QFrame>

class DropFrame : public QFrame
{
    Q_OBJECT
public:
    explicit DropFrame(QWidget *parent = 0);

    virtual void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    virtual void dropEvent(QDropEvent *) Q_DECL_OVERRIDE;
signals:
    void fileDrop(const QString file);

public slots:
};

#endif // DROPFRAME_H
