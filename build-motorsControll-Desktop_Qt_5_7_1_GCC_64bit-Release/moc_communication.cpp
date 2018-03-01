/****************************************************************************
** Meta object code from reading C++ file 'communication.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../motorsControl_udp/communication.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'communication.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Communication_t {
    QByteArrayData data[9];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Communication_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Communication_t qt_meta_stringdata_Communication = {
    {
QT_MOC_LITERAL(0, 0, 13), // "Communication"
QT_MOC_LITERAL(1, 14, 8), // "response"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 6), // "unitId"
QT_MOC_LITERAL(4, 31, 7), // "request"
QT_MOC_LITERAL(5, 39, 15), // "connectionError"
QT_MOC_LITERAL(6, 55, 7), // "errorId"
QT_MOC_LITERAL(7, 63, 8), // "errorStr"
QT_MOC_LITERAL(8, 72, 12) // "unitFinished"

    },
    "Communication\0response\0\0unitId\0request\0"
    "connectionError\0errorId\0errorStr\0"
    "unitFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Communication[] = {

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
       4,    1,   39,    2, 0x06 /* Public */,
       5,    3,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   49,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::UChar, QMetaType::QByteArray,    3,    1,
    QMetaType::Void, QMetaType::QByteArray,    4,
    QMetaType::Void, QMetaType::UChar, QMetaType::UShort, QMetaType::QString,    3,    6,    7,

 // slots: parameters
    QMetaType::Void, QMetaType::UChar,    3,

       0        // eod
};

void Communication::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Communication *_t = static_cast<Communication *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->response((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< const QByteArray(*)>(_a[2]))); break;
        case 1: _t->request((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 2: _t->connectionError((*reinterpret_cast< quint8(*)>(_a[1])),(*reinterpret_cast< quint16(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 3: _t->unitFinished((*reinterpret_cast< quint8(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Communication::*_t)(quint8 , const QByteArray );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Communication::response)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Communication::*_t)(const QByteArray & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Communication::request)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (Communication::*_t)(quint8 , quint16 , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Communication::connectionError)) {
                *result = 2;
                return;
            }
        }
    }
}

const QMetaObject Communication::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Communication.data,
      qt_meta_data_Communication,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Communication::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Communication::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Communication.stringdata0))
        return static_cast<void*>(const_cast< Communication*>(this));
    return QObject::qt_metacast(_clname);
}

int Communication::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void Communication::response(quint8 _t1, const QByteArray _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Communication::request(const QByteArray & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Communication::connectionError(quint8 _t1, quint16 _t2, QString _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
