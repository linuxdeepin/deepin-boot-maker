/****************************************************************************
** Meta object code from reading C++ file 'resultview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "view/resultview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'resultview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ResultView_t {
    QByteArrayData data[6];
    char stringdata0[62];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ResultView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ResultView_t qt_meta_stringdata_ResultView = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ResultView"
QT_MOC_LITERAL(1, 11, 18), // "onLogLinkActivated"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 12), // "updateResult"
QT_MOC_LITERAL(4, 44, 5), // "error"
QT_MOC_LITERAL(5, 50, 11) // "description"

    },
    "ResultView\0onLogLinkActivated\0\0"
    "updateResult\0error\0description"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ResultView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x0a /* Public */,
       3,    3,   27,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::UInt, QMetaType::QString, QMetaType::QString,    4,    2,    5,

       0        // eod
};

void ResultView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ResultView *_t = static_cast<ResultView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onLogLinkActivated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->updateResult((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ResultView::staticMetaObject = {
    { &DWidget::staticMetaObject, qt_meta_stringdata_ResultView.data,
      qt_meta_data_ResultView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ResultView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ResultView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ResultView.stringdata0))
        return static_cast<void*>(this);
    return DWidget::qt_metacast(_clname);
}

int ResultView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
