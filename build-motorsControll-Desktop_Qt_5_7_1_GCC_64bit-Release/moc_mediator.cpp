/****************************************************************************
** Meta object code from reading C++ file 'mediator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../motorsControl_udp/mediator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mediator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Mediator_t {
    QByteArrayData data[13];
    char stringdata0[116];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Mediator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Mediator_t qt_meta_stringdata_Mediator = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Mediator"
QT_MOC_LITERAL(1, 9, 8), // "response"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 7), // "nUnitId"
QT_MOC_LITERAL(4, 27, 3), // "buf"
QT_MOC_LITERAL(5, 31, 15), // "reconnectDevice"
QT_MOC_LITERAL(6, 47, 9), // "nDeviceId"
QT_MOC_LITERAL(7, 57, 10), // "errorOccur"
QT_MOC_LITERAL(8, 68, 7), // "errorId"
QT_MOC_LITERAL(9, 76, 8), // "errorStr"
QT_MOC_LITERAL(10, 85, 16), // "motorAttrChanged"
QT_MOC_LITERAL(11, 102, 7), // "nAttrId"
QT_MOC_LITERAL(12, 110, 5) // "value"

    },
    "Mediator\0response\0\0nUnitId\0buf\0"
    "reconnectDevice\0nDeviceId\0errorOccur\0"
    "errorId\0errorStr\0motorAttrChanged\0"
    "nAttrId\0value"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Mediator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x0a /* Public */,
       5,    1,   39,    2, 0x0a /* Public */,
       7,    3,   42,    2, 0x0a /* Public */,
      10,    3,   49,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::UChar, QMetaType::QByteArray,    3,    4,
    QMetaType::Void, QMetaType::UChar,    6,
    QMetaType::Void, QMetaType::UChar, QMetaType::UShort, QMetaType::QString,    6,    8,    9,
    QMetaType::Void, QMetaType::UChar, QMetaType::UChar, QMetaType::QVariant,    6,   11,   12,

       0        // eod
};

void Mediator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Mediator *_t = static_cast<Mediator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->response((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 1: _t->reconnectDevice((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        case 2: _t->errorOccur((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: _t->motorAttrChanged((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint8(*)>(_a[2])),(*reinterpret_cast< QVariant(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObject Mediator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Mediator.data,
      qt_meta_data_Mediator,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Mediator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Mediator::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Mediator.stringdata0))
        return static_cast<void*>(const_cast< Mediator*>(this));
    return QObject::qt_metacast(_clname);
}

int Mediator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
QT_END_MOC_NAMESPACE
