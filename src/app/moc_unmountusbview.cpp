/****************************************************************************
** Meta object code from reading C++ file 'unmountusbview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "view/unmountusbview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'unmountusbview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UnmountUsbView_t {
    QByteArrayData data[8];
    char stringdata0[74];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UnmountUsbView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UnmountUsbView_t qt_meta_stringdata_UnmountUsbView = {
    {
QT_MOC_LITERAL(0, 0, 14), // "UnmountUsbView"
QT_MOC_LITERAL(1, 15, 12), // "startSpinner"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 7), // "finish1"
QT_MOC_LITERAL(4, 37, 5), // "error"
QT_MOC_LITERAL(5, 43, 5), // "title"
QT_MOC_LITERAL(6, 49, 11), // "description"
QT_MOC_LITERAL(7, 61, 12) // "pauseSpinner"

    },
    "UnmountUsbView\0startSpinner\0\0finish1\0"
    "error\0title\0description\0pauseSpinner"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UnmountUsbView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   29,    2, 0x06 /* Public */,
       3,    3,   30,    2, 0x06 /* Public */,
       7,    0,   37,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::UInt, QMetaType::QString, QMetaType::QString,    4,    5,    6,
    QMetaType::Void,

       0        // eod
};

void UnmountUsbView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UnmountUsbView *_t = static_cast<UnmountUsbView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startSpinner(); break;
        case 1: _t->finish1((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 2: _t->pauseSpinner(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UnmountUsbView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UnmountUsbView::startSpinner)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UnmountUsbView::*)(quint32 , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UnmountUsbView::finish1)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UnmountUsbView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UnmountUsbView::pauseSpinner)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UnmountUsbView::staticMetaObject = {
    { &DWidget::staticMetaObject, qt_meta_stringdata_UnmountUsbView.data,
      qt_meta_data_UnmountUsbView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *UnmountUsbView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UnmountUsbView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UnmountUsbView.stringdata0))
        return static_cast<void*>(this);
    return DWidget::qt_metacast(_clname);
}

int UnmountUsbView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void UnmountUsbView::startSpinner()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void UnmountUsbView::finish1(quint32 _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UnmountUsbView::pauseSpinner()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE