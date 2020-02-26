/****************************************************************************
** Meta object code from reading C++ file 'bmdbusinterface.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "backend/bmdbusinterface.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bmdbusinterface.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BMDBusInterface_t {
    QByteArrayData data[26];
    char stringdata0[293];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BMDBusInterface_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BMDBusInterface_t qt_meta_stringdata_BMDBusInterface = {
    {
QT_MOC_LITERAL(0, 0, 15), // "BMDBusInterface"
QT_MOC_LITERAL(1, 16, 17), // "DeviceListChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 14), // "deviceListJson"
QT_MOC_LITERAL(4, 50, 8), // "Finished"
QT_MOC_LITERAL(5, 59, 7), // "errcode"
QT_MOC_LITERAL(6, 67, 11), // "description"
QT_MOC_LITERAL(7, 79, 14), // "ReportProgress"
QT_MOC_LITERAL(8, 94, 7), // "current"
QT_MOC_LITERAL(9, 102, 5), // "total"
QT_MOC_LITERAL(10, 108, 5), // "title"
QT_MOC_LITERAL(11, 114, 15), // "ReportProgress1"
QT_MOC_LITERAL(12, 130, 10), // "DeviceList"
QT_MOC_LITERAL(13, 141, 26), // "QDBusPendingReply<QString>"
QT_MOC_LITERAL(14, 168, 7), // "Install"
QT_MOC_LITERAL(15, 176, 23), // "QDBusPendingReply<bool>"
QT_MOC_LITERAL(16, 200, 5), // "image"
QT_MOC_LITERAL(17, 206, 6), // "device"
QT_MOC_LITERAL(18, 213, 9), // "partition"
QT_MOC_LITERAL(19, 223, 12), // "formatDevice"
QT_MOC_LITERAL(20, 236, 9), // "CheckFile"
QT_MOC_LITERAL(21, 246, 8), // "filepath"
QT_MOC_LITERAL(22, 255, 6), // "Reboot"
QT_MOC_LITERAL(23, 262, 19), // "QDBusPendingReply<>"
QT_MOC_LITERAL(24, 282, 5), // "Start"
QT_MOC_LITERAL(25, 288, 4) // "Stop"

    },
    "BMDBusInterface\0DeviceListChanged\0\0"
    "deviceListJson\0Finished\0errcode\0"
    "description\0ReportProgress\0current\0"
    "total\0title\0ReportProgress1\0DeviceList\0"
    "QDBusPendingReply<QString>\0Install\0"
    "QDBusPendingReply<bool>\0image\0device\0"
    "partition\0formatDevice\0CheckFile\0"
    "filepath\0Reboot\0QDBusPendingReply<>\0"
    "Start\0Stop"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BMDBusInterface[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    2,   67,    2, 0x06 /* Public */,
       7,    4,   72,    2, 0x06 /* Public */,
      11,    4,   81,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    0,   90,    2, 0x0a /* Public */,
      14,    4,   91,    2, 0x0a /* Public */,
      20,    1,  100,    2, 0x0a /* Public */,
      22,    0,  103,    2, 0x0a /* Public */,
      24,    0,  104,    2, 0x0a /* Public */,
      25,    0,  105,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString, QMetaType::QString,    8,    9,   10,    6,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString, QMetaType::QString,    8,    9,   10,    6,

 // slots: parameters
    0x80000000 | 13,
    0x80000000 | 15, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Bool,   16,   17,   18,   19,
    0x80000000 | 15, QMetaType::QString,   21,
    0x80000000 | 23,
    0x80000000 | 23,
    0x80000000 | 23,

       0        // eod
};

void BMDBusInterface::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BMDBusInterface *_t = static_cast<BMDBusInterface *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->DeviceListChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->Finished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->ReportProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 3: _t->ReportProgress1((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const QString(*)>(_a[4]))); break;
        case 4: { QDBusPendingReply<QString> _r = _t->DeviceList();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<QString>*>(_a[0]) = std::move(_r); }  break;
        case 5: { QDBusPendingReply<bool> _r = _t->Install((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = std::move(_r); }  break;
        case 6: { QDBusPendingReply<bool> _r = _t->CheckFile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<bool>*>(_a[0]) = std::move(_r); }  break;
        case 7: { QDBusPendingReply<> _r = _t->Reboot();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = std::move(_r); }  break;
        case 8: { QDBusPendingReply<> _r = _t->Start();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = std::move(_r); }  break;
        case 9: { QDBusPendingReply<> _r = _t->Stop();
            if (_a[0]) *reinterpret_cast< QDBusPendingReply<>*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (BMDBusInterface::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMDBusInterface::DeviceListChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (BMDBusInterface::*)(int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMDBusInterface::Finished)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (BMDBusInterface::*)(int , int , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMDBusInterface::ReportProgress)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (BMDBusInterface::*)(int , int , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&BMDBusInterface::ReportProgress1)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BMDBusInterface::staticMetaObject = {
    { &QDBusAbstractInterface::staticMetaObject, qt_meta_stringdata_BMDBusInterface.data,
      qt_meta_data_BMDBusInterface,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BMDBusInterface::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BMDBusInterface::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BMDBusInterface.stringdata0))
        return static_cast<void*>(this);
    return QDBusAbstractInterface::qt_metacast(_clname);
}

int BMDBusInterface::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDBusAbstractInterface::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void BMDBusInterface::DeviceListChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BMDBusInterface::Finished(int _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void BMDBusInterface::ReportProgress(int _t1, int _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void BMDBusInterface::ReportProgress1(int _t1, int _t2, const QString & _t3, const QString & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
