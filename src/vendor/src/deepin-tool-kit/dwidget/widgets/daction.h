#ifndef DACTION_H
#define DACTION_H

#include <DObject>
#include <dwidget_global.h>

#include <QAction>

DWIDGET_BEGIN_NAMESPACE

class DMenu;
class DActionPrivate;

class DAction : public QAction, public DObject
{
    Q_OBJECT

public:
    explicit DAction(QObject *parent);
    DAction(const QString &text, QObject* parent);
    DAction(const QIcon &icon, const QString &text, QObject* parent);

    ~DAction();

    DMenu *menu() const;
    void setMenu(DMenu *menu);

private:
    friend class DMenuPrivate;

    D_DECLARE_PRIVATE(DAction)
};

DWIDGET_END_NAMESPACE

#endif // DACTION_H
