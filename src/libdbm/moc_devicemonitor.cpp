/****************************************************************************
** Meta object code from reading C++ file 'devicemonitor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "util/devicemonitor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'devicemonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DeviceMonitor_t {
    QByteArrayData data[8];
    char stringdata0[102];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DeviceMonitor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DeviceMonitor_t qt_meta_stringdata_DeviceMonitor = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DeviceMonitor"
QT_MOC_LITERAL(1, 14, 12), // "startMonitor"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 12), // "pauseMonitor"
QT_MOC_LITERAL(4, 41, 26), // "removablePartitionsChanged"
QT_MOC_LITERAL(5, 68, 17), // "QList<DeviceInfo>"
QT_MOC_LITERAL(6, 86, 4), // "list"
QT_MOC_LITERAL(7, 91, 10) // "deviceList"

    },
    "DeviceMonitor\0startMonitor\0\0pauseMonitor\0"
    "removablePartitionsChanged\0QList<DeviceInfo>\0"
    "list\0deviceList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DeviceMonitor[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   34,    2, 0x06 /* Public */,
       3,    0,   35,    2, 0x06 /* Public */,
       4,    1,   36,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   39,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,

 // slots: parameters
    0x80000000 | 5,

       0        // eod
};

void DeviceMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DeviceMonitor *_t = static_cast<DeviceMonitor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->startMonitor(); break;
        case 1: _t->pauseMonitor(); break;
        case 2: _t->removablePartitionsChanged((*reinterpret_cast< const QList<DeviceInfo>(*)>(_a[1]))); break;
        case 3: { QList<DeviceInfo> _r = _t->deviceList();
            if (_a[0]) *reinterpret_cast< QList<DeviceInfo>*>(_a[0]) = std::move(_r); }  break;
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
            using _t = void (DeviceMonitor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceMonitor::startMonitor)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DeviceMonitor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceMonitor::pauseMonitor)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DeviceMonitor::*)(const QList<DeviceInfo> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DeviceMonitor::removablePartitionsChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DeviceMonitor::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_DeviceMonitor.data,
      qt_meta_data_DeviceMonitor,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *DeviceMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DeviceMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DeviceMonitor.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int DeviceMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void DeviceMonitor::startMonitor()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void DeviceMonitor::pauseMonitor()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void DeviceMonitor::removablePartitionsChanged(const QList<DeviceInfo> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
