/****************************************************************************
** Meta object code from reading C++ file 'bminterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "bminterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bminterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BMInterface_t {
    QByteArrayData data[24];
    char stringdata0[237];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BMInterface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BMInterface_t qt_meta_stringdata_BMInterface = {
    {
QT_MOC_LITERAL(0, 0, 11), // "BMInterface"
QT_MOC_LITERAL(1, 12, 17), // "deviceListChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 17), // "QList<DeviceInfo>"
QT_MOC_LITERAL(4, 49, 10), // "partitions"
QT_MOC_LITERAL(5, 60, 8), // "finished"
QT_MOC_LITERAL(6, 69, 7), // "errcode"
QT_MOC_LITERAL(7, 77, 11), // "description"
QT_MOC_LITERAL(8, 89, 14), // "reportProgress"
QT_MOC_LITERAL(9, 104, 7), // "current"
QT_MOC_LITERAL(10, 112, 5), // "total"
QT_MOC_LITERAL(11, 118, 5), // "title"
QT_MOC_LITERAL(12, 124, 15), // "reportProgress1"
QT_MOC_LITERAL(13, 140, 12), // "startInstall"
QT_MOC_LITERAL(14, 153, 5), // "image"
QT_MOC_LITERAL(15, 159, 6), // "device"
QT_MOC_LITERAL(16, 166, 9), // "partition"
QT_MOC_LITERAL(17, 176, 12), // "formatDevice"
QT_MOC_LITERAL(18, 189, 5), // "start"
QT_MOC_LITERAL(19, 195, 4), // "stop"
QT_MOC_LITERAL(20, 200, 6), // "reboot"
QT_MOC_LITERAL(21, 207, 10), // "deviceList"
QT_MOC_LITERAL(22, 218, 9), // "checkfile"
QT_MOC_LITERAL(23, 228, 8) // "filepath"

    },
    "BMInterface\0deviceListChanged\0\0"
    "QList<DeviceInfo>\0partitions\0finished\0"
    "errcode\0description\0reportProgress\0"
    "current\0total\0title\0reportProgress1\0"
    "startInstall\0image\0device\0partition\0"
    "formatDevice\0start\0stop\0reboot\0"
    "deviceList\0checkfile\0filepath"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BMInterface[] = {

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
       1,    1,   64,    2, 0x06 /* Public */,
       5,    2,   67,    2, 0x06 /* Public */,
       8,    4,   72,    2, 0x06 /* Public */,
      12,    4,   81,    2, 0x06 /* Public */,
      13,    4,   90,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      18,    0,   99,    2, 0x0a /* Public */,
      19,    0,  100,    2, 0x0a /* Public */,
      20,    0,  101,    2, 0x0a /* Public */,
      21,    0,  102,    2, 0x0a /* Public */,
      22,    1,  103,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    6,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString, QMetaType::QString,    9,   10,   11,    7,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString, QMetaType::QString,    9,   10,   11,    7,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Bool,   14,   15,   16,   17,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 3,
    QMetaType::Bool, QMetaType::QString,   23,

       0        // eod
};

void BMInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BMInterface *_t = static_cast<BMInterface *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->deviceListChanged((*reinterpret_cast< const QList<DeviceInfo>(*)>(_a[1]))); break;
        case 1: _t->finished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->reportProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 3: _t->reportProgress1((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 4: _t->startInstall((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4]))); break;
        case 5: _t->start(); break;
        case 6: _t->stop(); break;
        case 7: _t->reboot(); break;
        case 8: { QList<DeviceInfo> _r = _t->deviceList();
            if (_a[0]) *reinterpret_cast< QList<DeviceInfo>*>(_a[0]) = std::move(_r); }  break;
        case 9: { bool _r = _t->checkfile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
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
            using _t = void (BMInterface::*)(const QList<DeviceInfo> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMInterface::deviceListChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BMInterface::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMInterface::finished)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BMInterface::*)(int , int , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMInterface::reportProgress)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BMInterface::*)(int , int , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMInterface::reportProgress1)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (BMInterface::*)(const QString & , const QString & , const QString & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMInterface::startInstall)) {
                *result = 4;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BMInterface::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BMInterface.data,
      qt_meta_data_BMInterface,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BMInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BMInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BMInterface.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Dtk::Core::DSingleton<BMInterface>"))
        return static_cast< Dtk::Core::DSingleton<BMInterface>*>(this);
    return QObject::qt_metacast(_clname);
}

int BMInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void BMInterface::deviceListChanged(const QList<DeviceInfo> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BMInterface::finished(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BMInterface::reportProgress(int _t1, int _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BMInterface::reportProgress1(int _t1, int _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void BMInterface::startInstall(const QString & _t1, const QString & _t2, const QString & _t3, bool _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
