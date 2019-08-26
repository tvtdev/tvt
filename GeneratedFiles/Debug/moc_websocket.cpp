/****************************************************************************
** Meta object code from reading C++ file 'websocket.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../websocket.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'websocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WebSocket_t {
    QByteArrayData data[15];
    char stringdata0[188];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WebSocket_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WebSocket_t qt_meta_stringdata_WebSocket = {
    {
QT_MOC_LITERAL(0, 0, 9), // "WebSocket"
QT_MOC_LITERAL(1, 10, 15), // "signalConnected"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 18), // "signalDisconnected"
QT_MOC_LITERAL(4, 46, 17), // "signalMsgRecieved"
QT_MOC_LITERAL(5, 64, 3), // "msg"
QT_MOC_LITERAL(6, 68, 23), // "signalCancelOrderResult"
QT_MOC_LITERAL(7, 92, 9), // "isSuccess"
QT_MOC_LITERAL(8, 102, 11), // "onConnected"
QT_MOC_LITERAL(9, 114, 14), // "onDisconnected"
QT_MOC_LITERAL(10, 129, 17), // "onTextMsgRecieved"
QT_MOC_LITERAL(11, 147, 11), // "onSslErrors"
QT_MOC_LITERAL(12, 159, 16), // "QList<QSslError>"
QT_MOC_LITERAL(13, 176, 6), // "errors"
QT_MOC_LITERAL(14, 183, 4) // "ping"

    },
    "WebSocket\0signalConnected\0\0"
    "signalDisconnected\0signalMsgRecieved\0"
    "msg\0signalCancelOrderResult\0isSuccess\0"
    "onConnected\0onDisconnected\0onTextMsgRecieved\0"
    "onSslErrors\0QList<QSslError>\0errors\0"
    "ping"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WebSocket[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    1,   61,    2, 0x06 /* Public */,
       6,    1,   64,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,   67,    2, 0x08 /* Private */,
       9,    0,   68,    2, 0x08 /* Private */,
      10,    1,   69,    2, 0x08 /* Private */,
      11,    1,   72,    2, 0x08 /* Private */,
      14,    0,   75,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Bool,    7,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,

       0        // eod
};

void WebSocket::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        WebSocket *_t = static_cast<WebSocket *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalConnected(); break;
        case 1: _t->signalDisconnected(); break;
        case 2: _t->signalMsgRecieved((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->signalCancelOrderResult((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->onConnected(); break;
        case 5: _t->onDisconnected(); break;
        case 6: _t->onTextMsgRecieved((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->onSslErrors((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 8: _t->ping(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QSslError> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (WebSocket::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WebSocket::signalConnected)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (WebSocket::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WebSocket::signalDisconnected)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (WebSocket::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WebSocket::signalMsgRecieved)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (WebSocket::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&WebSocket::signalCancelOrderResult)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject WebSocket::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_WebSocket.data,
      qt_meta_data_WebSocket,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *WebSocket::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WebSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_WebSocket.stringdata0))
        return static_cast<void*>(const_cast< WebSocket*>(this));
    return QObject::qt_metacast(_clname);
}

int WebSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void WebSocket::signalConnected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, Q_NULLPTR);
}

// SIGNAL 1
void WebSocket::signalDisconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, Q_NULLPTR);
}

// SIGNAL 2
void WebSocket::signalMsgRecieved(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void WebSocket::signalCancelOrderResult(bool _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
