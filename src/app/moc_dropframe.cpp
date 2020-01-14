/****************************************************************************
** Meta object code from reading C++ file 'dropframe.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "view/dropframe.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'dropframe.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DropFrame_t {
    QByteArrayData data[6];
    char stringdata0[52];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DropFrame_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DropFrame_t qt_meta_stringdata_DropFrame = {
    {
QT_MOC_LITERAL(0, 0, 9), // "DropFrame"
QT_MOC_LITERAL(1, 10, 15), // "fileAboutAccept"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 10), // "fileCancel"
QT_MOC_LITERAL(4, 38, 8), // "fileDrop"
QT_MOC_LITERAL(5, 47, 4) // "file"

    },
    "DropFrame\0fileAboutAccept\0\0fileCancel\0"
    "fileDrop\0file"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DropFrame[] = {

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
       3,    0,   30,    2, 0x06 /* Public */,
       4,    1,   31,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,

       0        // eod
};

void DropFrame::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DropFrame *_t = static_cast<DropFrame *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->fileAboutAccept(); break;
        case 1: _t->fileCancel(); break;
        case 2: _t->fileDrop((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DropFrame::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DropFrame::fileAboutAccept)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DropFrame::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DropFrame::fileCancel)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DropFrame::*)(const QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DropFrame::fileDrop)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DropFrame::staticMetaObject = {
    { &DFrame::staticMetaObject, qt_meta_stringdata_DropFrame.data,
      qt_meta_data_DropFrame,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DropFrame::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DropFrame::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DropFrame.stringdata0))
        return static_cast<void*>(this);
    return DFrame::qt_metacast(_clname);
}

int DropFrame::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DFrame::qt_metacall(_c, _id, _a);
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
void DropFrame::fileAboutAccept()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DropFrame::fileCancel()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DropFrame::fileDrop(const QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
