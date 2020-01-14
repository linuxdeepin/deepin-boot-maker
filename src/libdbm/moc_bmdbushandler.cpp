/****************************************************************************
** Meta object code from reading C++ file 'bmdbushandler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "backend/bmdbushandler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bmdbushandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_BMDbusHandler_t {
    QByteArrayData data[14];
    char stringdata0[125];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_BMDbusHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_BMDbusHandler_t qt_meta_stringdata_BMDbusHandler = {
    {
QT_MOC_LITERAL(0, 0, 13), // "BMDbusHandler"
QT_MOC_LITERAL(1, 14, 6), // "reboot"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 5), // "start"
QT_MOC_LITERAL(4, 28, 4), // "stop"
QT_MOC_LITERAL(5, 33, 10), // "deviceList"
QT_MOC_LITERAL(6, 44, 17), // "QList<DeviceInfo>"
QT_MOC_LITERAL(7, 62, 7), // "install"
QT_MOC_LITERAL(8, 70, 5), // "image"
QT_MOC_LITERAL(9, 76, 6), // "device"
QT_MOC_LITERAL(10, 83, 9), // "partition"
QT_MOC_LITERAL(11, 93, 12), // "formatDevice"
QT_MOC_LITERAL(12, 106, 9), // "checkfile"
QT_MOC_LITERAL(13, 116, 8) // "filepath"

    },
    "BMDbusHandler\0reboot\0\0start\0stop\0"
    "deviceList\0QList<DeviceInfo>\0install\0"
    "image\0device\0partition\0formatDevice\0"
    "checkfile\0filepath"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_BMDbusHandler[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    0,   46,    2, 0x0a /* Public */,
       5,    0,   47,    2, 0x0a /* Public */,
       7,    4,   48,    2, 0x0a /* Public */,
      12,    1,   57,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    0x80000000 | 6,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Bool,    8,    9,   10,   11,
    QMetaType::Bool, QMetaType::QString,   13,

       0        // eod
};

void BMDbusHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        BMDbusHandler *_t = static_cast<BMDbusHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->reboot(); break;
        case 1: _t->start(); break;
        case 2: _t->stop(); break;
        case 3: { QList<DeviceInfo> _r = _t->deviceList();
            if (_a[0]) *reinterpret_cast< QList<DeviceInfo>*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->install((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< bool(*)>(_a[4])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 5: { bool _r = _t->checkfile((*reinterpret_cast< const QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject BMDbusHandler::staticMetaObject = {
    { &BMHandler::staticMetaObject, qt_meta_stringdata_BMDbusHandler.data,
      qt_meta_data_BMDbusHandler,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *BMDbusHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *BMDbusHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_BMDbusHandler.stringdata0))
        return static_cast<void*>(this);
    return BMHandler::qt_metacast(_clname);
}

int BMDbusHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = BMHandler::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
