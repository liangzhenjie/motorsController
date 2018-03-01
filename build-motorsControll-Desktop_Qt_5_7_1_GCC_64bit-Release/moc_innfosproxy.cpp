/****************************************************************************
** Meta object code from reading C++ file 'innfosproxy.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../motorsControl_udp/innfosproxy.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'innfosproxy.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_InnfosProxy_t {
    QByteArrayData data[3];
    char stringdata0[18];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_InnfosProxy_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_InnfosProxy_t qt_meta_stringdata_InnfosProxy = {
    {
QT_MOC_LITERAL(0, 0, 11), // "InnfosProxy"
QT_MOC_LITERAL(1, 12, 4), // "Send"
QT_MOC_LITERAL(2, 17, 0) // ""

    },
    "InnfosProxy\0Send\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_InnfosProxy[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void InnfosProxy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        InnfosProxy *_t = static_cast<InnfosProxy *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Send(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject InnfosProxy::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_InnfosProxy.data,
      qt_meta_data_InnfosProxy,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *InnfosProxy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *InnfosProxy::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_InnfosProxy.stringdata0))
        return static_cast<void*>(const_cast< InnfosProxy*>(this));
    return QObject::qt_metacast(_clname);
}

int InnfosProxy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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
struct qt_meta_stringdata_NoDataProxy_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NoDataProxy_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NoDataProxy_t qt_meta_stringdata_NoDataProxy = {
    {
QT_MOC_LITERAL(0, 0, 11) // "NoDataProxy"

    },
    "NoDataProxy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NoDataProxy[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void NoDataProxy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject NoDataProxy::staticMetaObject = {
    { &InnfosProxy::staticMetaObject, qt_meta_stringdata_NoDataProxy.data,
      qt_meta_data_NoDataProxy,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NoDataProxy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoDataProxy::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NoDataProxy.stringdata0))
        return static_cast<void*>(const_cast< NoDataProxy*>(this));
    return InnfosProxy::qt_metacast(_clname);
}

int NoDataProxy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = InnfosProxy::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_NoCrcProxy_t {
    QByteArrayData data[1];
    char stringdata0[11];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NoCrcProxy_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NoCrcProxy_t qt_meta_stringdata_NoCrcProxy = {
    {
QT_MOC_LITERAL(0, 0, 10) // "NoCrcProxy"

    },
    "NoCrcProxy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NoCrcProxy[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void NoCrcProxy::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject NoCrcProxy::staticMetaObject = {
    { &InnfosProxy::staticMetaObject, qt_meta_stringdata_NoCrcProxy.data,
      qt_meta_data_NoCrcProxy,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NoCrcProxy::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NoCrcProxy::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NoCrcProxy.stringdata0))
        return static_cast<void*>(const_cast< NoCrcProxy*>(this));
    return InnfosProxy::qt_metacast(_clname);
}

int NoCrcProxy::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = InnfosProxy::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
struct qt_meta_stringdata_ProxyWatcher_t {
    QByteArrayData data[1];
    char stringdata0[13];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ProxyWatcher_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ProxyWatcher_t qt_meta_stringdata_ProxyWatcher = {
    {
QT_MOC_LITERAL(0, 0, 12) // "ProxyWatcher"

    },
    "ProxyWatcher"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ProxyWatcher[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void ProxyWatcher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject ProxyWatcher::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ProxyWatcher.data,
      qt_meta_data_ProxyWatcher,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ProxyWatcher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProxyWatcher::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ProxyWatcher.stringdata0))
        return static_cast<void*>(const_cast< ProxyWatcher*>(this));
    return QObject::qt_metacast(_clname);
}

int ProxyWatcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
