/****************************************************************************
** Meta object code from reading C++ file 'usbcreator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "usbcreator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'usbcreator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_UsbCreator_t {
    QByteArrayData data[11];
    char stringdata[105];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UsbCreator_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UsbCreator_t qt_meta_stringdata_UsbCreator = {
    {
QT_MOC_LITERAL(0, 0, 10),
QT_MOC_LITERAL(1, 11, 13),
QT_MOC_LITERAL(2, 25, 0),
QT_MOC_LITERAL(3, 26, 5),
QT_MOC_LITERAL(4, 32, 7),
QT_MOC_LITERAL(5, 40, 9),
QT_MOC_LITERAL(6, 50, 8),
QT_MOC_LITERAL(7, 59, 11),
QT_MOC_LITERAL(8, 71, 8),
QT_MOC_LITERAL(9, 80, 10),
QT_MOC_LITERAL(10, 91, 13)
    },
    "UsbCreator\0listUsbDrives\0\0start\0isoPath\0"
    "usbDriver\0biosMode\0processRate\0isFinish\0"
    "isISOImage\0url2LocalFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UsbCreator[] = {

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

 // methods: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x02 /* Public */,
       3,    3,   45,    2, 0x02 /* Public */,
       7,    0,   52,    2, 0x02 /* Public */,
       8,    0,   53,    2, 0x02 /* Public */,
       9,    1,   54,    2, 0x02 /* Public */,
      10,    1,   57,    2, 0x02 /* Public */,

 // methods: parameters
    QMetaType::QStringList,
    QMetaType::Int, QMetaType::QString, QMetaType::QString, QMetaType::Bool,    4,    5,    6,
    QMetaType::Int,
    QMetaType::Bool,
    QMetaType::Bool, QMetaType::QString,    4,
    QMetaType::QString, QMetaType::QString,    4,

       0        // eod
};

void UsbCreator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        UsbCreator *_t = static_cast<UsbCreator *>(_o);
        switch (_id) {
        case 0: { QStringList _r = _t->listUsbDrives();
            if (_a[0]) *reinterpret_cast< QStringList*>(_a[0]) = _r; }  break;
        case 1: { int _r = _t->start((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { int _r = _t->processRate();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { bool _r = _t->isFinish();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: { bool _r = _t->isISOImage((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { QString _r = _t->url2LocalFile((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObject UsbCreator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_UsbCreator.data,
      qt_meta_data_UsbCreator,  qt_static_metacall, 0, 0}
};


const QMetaObject *UsbCreator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UsbCreator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_UsbCreator.stringdata))
        return static_cast<void*>(const_cast< UsbCreator*>(this));
    return QObject::qt_metacast(_clname);
}

int UsbCreator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
