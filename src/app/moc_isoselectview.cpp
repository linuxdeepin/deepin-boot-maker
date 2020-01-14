/****************************************************************************
** Meta object code from reading C++ file 'isoselectview.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "view/isoselectview.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'isoselectview.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ThreadCheckFile_t {
    QByteArrayData data[3];
    char stringdata0[33];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ThreadCheckFile_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ThreadCheckFile_t qt_meta_stringdata_ThreadCheckFile = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ThreadCheckFile"
QT_MOC_LITERAL(1, 16, 15), // "checkFileFinish"
QT_MOC_LITERAL(2, 32, 0) // ""

    },
    "ThreadCheckFile\0checkFileFinish\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ThreadCheckFile[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    2,

       0        // eod
};

void ThreadCheckFile::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ThreadCheckFile *_t = static_cast<ThreadCheckFile *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->checkFileFinish((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ThreadCheckFile::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ThreadCheckFile::checkFileFinish)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ThreadCheckFile::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_ThreadCheckFile.data,
      qt_meta_data_ThreadCheckFile,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ThreadCheckFile::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ThreadCheckFile::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ThreadCheckFile.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int ThreadCheckFile::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void ThreadCheckFile::checkFileFinish(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
struct qt_meta_stringdata_ISOSelectView_t {
    QByteArrayData data[10];
    char stringdata0[125];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ISOSelectView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ISOSelectView_t qt_meta_stringdata_ISOSelectView = {
    {
QT_MOC_LITERAL(0, 0, 13), // "ISOSelectView"
QT_MOC_LITERAL(1, 14, 20), // "requestVerfiyISOFile"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 4), // "file"
QT_MOC_LITERAL(4, 41, 15), // "isoFileSelected"
QT_MOC_LITERAL(5, 57, 14), // "onFileSelected"
QT_MOC_LITERAL(6, 72, 20), // "onFileVerfiyFinished"
QT_MOC_LITERAL(7, 93, 2), // "ok"
QT_MOC_LITERAL(8, 96, 16), // "slot_ThemeChange"
QT_MOC_LITERAL(9, 113, 11) // "isoFilePath"

    },
    "ISOSelectView\0requestVerfiyISOFile\0\0"
    "file\0isoFileSelected\0onFileSelected\0"
    "onFileVerfiyFinished\0ok\0slot_ThemeChange\0"
    "isoFilePath"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ISOSelectView[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       1,   50, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    0,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   43,    2, 0x0a /* Public */,
       6,    1,   46,    2, 0x0a /* Public */,
       8,    0,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Bool,    7,
    QMetaType::Void,

 // properties: name, type, flags
       9, QMetaType::QString, 0x00095001,

       0        // eod
};

void ISOSelectView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ISOSelectView *_t = static_cast<ISOSelectView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->requestVerfiyISOFile((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->isoFileSelected(); break;
        case 2: _t->onFileSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onFileVerfiyFinished((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->slot_ThemeChange(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (ISOSelectView::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ISOSelectView::requestVerfiyISOFile)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (ISOSelectView::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&ISOSelectView::isoFileSelected)) {
                *result = 1;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        ISOSelectView *_t = static_cast<ISOSelectView *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->isoFilePath(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject ISOSelectView::staticMetaObject = {
    { &DWidget::staticMetaObject, qt_meta_stringdata_ISOSelectView.data,
      qt_meta_data_ISOSelectView,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ISOSelectView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ISOSelectView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ISOSelectView.stringdata0))
        return static_cast<void*>(this);
    return DWidget::qt_metacast(_clname);
}

int ISOSelectView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
#ifndef QT_NO_PROPERTIES
   else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 1;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 1;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void ISOSelectView::requestVerfiyISOFile(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ISOSelectView::isoFileSelected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
