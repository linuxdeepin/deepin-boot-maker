#pragma once

#include <DObject>
#include <DAction>
#include <dwidget_global.h>

#include <QObject>

DWIDGET_BEGIN_NAMESPACE

class DMenuPrivate;
class DMenuItem;

class LIBDTKWIDGETSHARED_EXPORT DMenu:  public QObject, public DObject
{
    Q_OBJECT

public:
    enum Direction
    {
        Top,
        Left,
        Bottom,
        Right,
    };

    explicit DMenu(QObject *parent = Q_NULLPTR);
    void attatch(QWidget *);

    void setDockMenu(const bool isDockMenu);
    bool isDockMenu() const;

    void setDirection(const Direction direction);
    Direction direction() const;

    DAction *addAction(const QString & text);
    DAction *addAction(const QIcon & icon, const QString & text);
    void addAction(DAction *action);

    DAction *addMenu(DMenu *menu);
    DMenu *addMenu(const QString & title);
    DMenu *addMenu(const QIcon & icon, const QString & title);
    DAction *addSeparator();

    DAction *actionAt(const QString &text) const;
    DAction *actionAt(int index) const;
    QList<DAction*> actionList() const;

    DAction *exec();
    DAction *exec(const QPoint & p, DAction *action = 0);
    bool popup(const QPoint &pos, DAction *action = 0);

Q_SIGNALS:
    void triggered(DAction * action);

private:
    friend class DActionPrivate;
    D_DECLARE_PRIVATE(DMenu)
    Q_DISABLE_COPY(DMenu)
};

DWIDGET_END_NAMESPACE
