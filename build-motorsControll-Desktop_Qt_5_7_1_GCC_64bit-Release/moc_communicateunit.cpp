/****************************************************************************
** Meta object code from reading C++ file 'communicateunit.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../motorsControl_udp/communicateunit.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'communicateunit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CommunicateUnit_t {
    QByteArrayData data[9];
    char stringdata0[82];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CommunicateUnit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CommunicateUnit_t qt_meta_stringdata_CommunicateUnit = {
    {
QT_MOC_LITERAL(0, 0, 15), // "CommunicateUnit"
QT_MOC_LITERAL(1, 16, 8), // "response"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 6), // "unitId"
QT_MOC_LITERAL(4, 33, 12), // "responseData"
QT_MOC_LITERAL(5, 46, 5), // "error"
QT_MOC_LITERAL(6, 52, 11), // "errorString"
QT_MOC_LITERAL(7, 64, 8), // "finished"
QT_MOC_LITERAL(8, 73, 8) // "progress"

    },
    "CommunicateUnit\0response\0\0unitId\0"
    "responseData\0error\0errorString\0finished\0"
    "progress"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CommunicateUnit[] = {

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
       1,    2,   34,    2, 0x06 /* Public */,
       5,    1,   39,    2, 0x06 /* Public */,
       7,    1,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   45,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::UInt, QMetaType::QByteArray,    3,    4,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void, QMetaType::UInt,    3,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void CommunicateUnit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CommunicateUnit *_t = static_cast<CommunicateUnit *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->response((*reinterpret_cast< quint32(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2]))); break;
        case 1: _t->error((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->finished((*reinterpret_cast< quint32(*)>(_a[1]))); break;
        case 3: _t->progress(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (CommunicateUnit::*_t)(quint32 , QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicateUnit::response)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (CommunicateUnit::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicateUnit::error)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (CommunicateUnit::*_t)(quint32 );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&CommunicateUnit::finished)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject CommunicateUnit::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CommunicateUnit.data,
      qt_meta_data_CommunicateUnit,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *CommunicateUnit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CommunicateUnit::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_CommunicateUnit.stringdata0))
        return static_cast<void*>(const_cast< CommunicateUnit*>(this));
    return QObject::qt_metacast(_clname);
}

int CommunicateUnit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void CommunicateUnit::response(quint32 _t1, QByteArray _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CommunicateUnit::error(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void CommunicateUnit::finished(quint32 _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
