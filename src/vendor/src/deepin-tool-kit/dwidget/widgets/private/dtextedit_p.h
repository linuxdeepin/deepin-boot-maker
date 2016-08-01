#ifndef DTEXTEDIT_P_H
#define DTEXTEDIT_P_H

#include "dtextedit.h"

#include <private/dobject_p.h>

DWIDGET_BEGIN_NAMESPACE

class DTextEditPrivate : public DObjectPrivate
{
    DTextEditPrivate(DTextEdit *qq);

    void init();

    QFrame *insideFrame;

    D_DECLARE_PUBLIC(DTextEdit)
};

DWIDGET_END_NAMESPACE

#endif // DTEXTEDIT_P_H
