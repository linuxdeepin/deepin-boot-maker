#ifndef DTEXTEDIT_H
#define DTEXTEDIT_H

#include "dwidget_global.h"

#include <dobject.h>

#include <QTextEdit>

DWIDGET_BEGIN_NAMESPACE

class DTextEditPrivate;
class DTextEdit : public QTextEdit, public DObject
{
    Q_OBJECT

public:
    explicit DTextEdit(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

private:
    D_DECLARE_PRIVATE(DTextEdit)
    Q_DISABLE_COPY(DTextEdit)
};

DWIDGET_END_NAMESPACE

#endif // DTEXTEDIT_H
