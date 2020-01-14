/****************************************************************************
** Meta object code from reading C++ file 'bmhandler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "backend/bmhandler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bmhandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BMHandler_t {
    QByteArrayData data[20];
    char stringdata0[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BMHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BMHandler_t qt_meta_stringdata_BMHandler = {
    {
QT_MOC_LITERAL(0, 0, 9), // "BMHandler"
QT_MOC_LITERAL(1, 10, 12), // "startInstall"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 14), // "startCheckfile"
QT_MOC_LITERAL(4, 39, 8), // "filepath"
QT_MOC_LITERAL(5, 48, 26), // "removablePartitionsChanged"
QT_MOC_LITERAL(6, 75, 17), // "QList<DeviceInfo>"
QT_MOC_LITERAL(7, 93, 4), // "list"
QT_MOC_LITERAL(8, 98, 8), // "finished"
QT_MOC_LITERAL(9, 107, 7), // "errcode"
QT_MOC_LITERAL(10, 115, 11), // "description"
QT_MOC_LITERAL(11, 127, 14), // "reportProgress"
QT_MOC_LITERAL(12, 142, 7), // "current"
QT_MOC_LITERAL(13, 150, 5), // "error"
QT_MOC_LITERAL(14, 156, 5), // "title"
QT_MOC_LITERAL(15, 162, 6), // "reboot"
QT_MOC_LITERAL(16, 169, 5), // "start"
QT_MOC_LITERAL(17, 175, 4), // "stop"
QT_MOC_LITERAL(18, 180, 10), // "deviceList"
QT_MOC_LITERAL(19, 191, 9) // "checkfile"

    },
    "BMHandler\0startInstall\0\0startCheckfile\0"
    "filepath\0removablePartitionsChanged\0"
    "QList<DeviceInfo>\0list\0finished\0errcode\0"
    "description\0reportProgress\0current\0"
    "error\0title\0reboot\0start\0stop\0deviceList\0"
    "checkfile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BMHandler[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    4,   64,    2, 0x06 /* Public */,
       3,    1,   73,    2, 0x06 /* Public */,
       5,    1,   76,    2, 0x06 /* Public */,
       8,    2,   79,    2, 0x06 /* Public */,
      11,    4,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,   93,    2, 0x0a /* Public */,
      16,    0,   94,    2, 0x0a /* Public */,
      17,    0,   95,    2, 0x0a /* Public */,
      18,    0,   96,    2, 0x0a /* Public */,
      19,    1,   97,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Bool,    2,    2,    2,    2,
    QMetaType::Void, QMetaType::QString,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    9,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString, QMetaType::QString,   12,   13,   14,   10,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 6,
    QMetaType::Bool, QMetaType::QString,    4,

       0        // eod
};

void BMHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BMHandler *_t = static_cast<BMHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startInstall((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 1: _t->startCheckfile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->removablePartitionsChanged((*reinterpret_cast< const QList<DeviceInfo>(*)>(_a[1]))); break;
        case 3: _t->finished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 4: _t->reportProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 5: _t->reboot(); break;
        case 6: _t->start(); break;
        case 7: _t->stop(); break;
        case 8: { QList<DeviceInfo> _r = _t->deviceList();
            if (_a[0]) *reinterpret_cast< QList<DeviceInfo>*>(_a[0]) = std::move(_r); }  break;
        case 9: { bool _r = _t->checkfile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<DeviceInfo> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BMHandler::*)(const QString & , const QString & , const QString & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMHandler::startInstall)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BMHandler::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMHandler::startCheckfile)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BMHandler::*)(const QList<DeviceInfo> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMHandler::removablePartitionsChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BMHandler::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMHandler::finished)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BMHandler::*)(int , int , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMHandler::reportProgress)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BMHandler::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BMHandler.data,
      qt_meta_data_BMHandler,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BMHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BMHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BMHandler.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int BMHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void BMHandler::startInstall(const QString & _t1, const QString & _t2, const QString & _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BMHandler::startCheckfile(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BMHandler::removablePartitionsChanged(const QList<DeviceInfo> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BMHandler::finished(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BMHandler::reportProgress(int _t1, int _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
