#pragma once

#include <QMenu>

#include "dobject.h"
#include "dwidget_global.h"

DWIDGET_BEGIN_NAMESPACE
class DAction;
class DMenuPrivate;

class DMenu : public QMenu, public DObject
{
    Q_OBJECT

public:
    explicit DMenu(QWidget *parent = 0);

    void attatch(QWidget *){/*  QMenu:: setParent(w);*/}

    DAction *addAction(const QString &text);
    DAction *addAction(const QIcon & icon, const QString & text);
    void addAction(DAction *action);

    DAction *addMenu(DMenu *menu);
    DMenu *addMenu(const QString &title);
    DMenu *addMenu(const QIcon & icon, const QString & title);
    DAction *addSeparator();

//    DAction *actionAt(const QString &text) const;
//    DAction *actionAt(int index) const;

Q_SIGNALS:
    void triggered(DAction *action);

private:
    friend class DActionPrivate;
private:
    Q_DISABLE_COPY(DMenu)
    D_DECLARE_PRIVATE(DMenu)
};

DWIDGET_END_NAMESPACE
