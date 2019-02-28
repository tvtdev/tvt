/****************************************************************************
** Meta object code from reading C++ file 'qimap.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/qimap.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qimap.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QImap_t {
    QByteArrayData data[16];
    char stringdata0[188];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QImap_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QImap_t qt_meta_stringdata_QImap = {
    {
QT_MOC_LITERAL(0, 0, 5), // "QImap"
QT_MOC_LITERAL(1, 6, 16), // "signalCapalibily"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 7), // "content"
QT_MOC_LITERAL(4, 32, 11), // "signalLogin"
QT_MOC_LITERAL(5, 44, 12), // "signalLogout"
QT_MOC_LITERAL(6, 57, 10), // "signalList"
QT_MOC_LITERAL(7, 68, 12), // "signalStatus"
QT_MOC_LITERAL(8, 81, 12), // "signalSelect"
QT_MOC_LITERAL(9, 94, 12), // "signalSearch"
QT_MOC_LITERAL(10, 107, 13), // "signalExamine"
QT_MOC_LITERAL(11, 121, 11), // "signalFetch"
QT_MOC_LITERAL(12, 133, 15), // "signalUidSearch"
QT_MOC_LITERAL(13, 149, 15), // "signalConnected"
QT_MOC_LITERAL(14, 165, 12), // "signalUnkown"
QT_MOC_LITERAL(15, 178, 9) // "readReady"

    },
    "QImap\0signalCapalibily\0\0content\0"
    "signalLogin\0signalLogout\0signalList\0"
    "signalStatus\0signalSelect\0signalSearch\0"
    "signalExamine\0signalFetch\0signalUidSearch\0"
    "signalConnected\0signalUnkown\0readReady"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QImap[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,
       5,    0,   85,    2, 0x06 /* Public */,
       6,    1,   86,    2, 0x06 /* Public */,
       7,    1,   89,    2, 0x06 /* Public */,
       8,    1,   92,    2, 0x06 /* Public */,
       9,    1,   95,    2, 0x06 /* Public */,
      10,    1,   98,    2, 0x06 /* Public */,
      11,    1,  101,    2, 0x06 /* Public */,
      12,    1,  104,    2, 0x06 /* Public */,
      13,    1,  107,    2, 0x06 /* Public */,
      14,    1,  110,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    0,  113,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void QImap::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QImap *_t = static_cast<QImap *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalCapalibily((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->signalLogin((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->signalLogout(); break;
        case 3: _t->signalList((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->signalStatus((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->signalSelect((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: _t->signalSearch((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->signalExamine((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: _t->signalFetch((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->signalUidSearch((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 10: _t->signalConnected((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 11: _t->signalUnkown((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 12: _t->readReady(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (QImap::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QImap::signalCapalibily)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (QImap::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QImap::signalLogin)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (QImap::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QImap::signalLogout)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (QImap::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QImap::signalList)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (QImap::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QImap::signalStatus)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (QImap::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QImap::signalSelect)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (QImap::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QImap::signalSearch)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (QImap::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QImap::signalExamine)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (QImap::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QImap::signalFetch)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (QImap::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QImap::signalUidSearch)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (QImap::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QImap::signalConnected)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (QImap::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&QImap::signalUnkown)) {
                *result = 11;
                return;
            }
        }
    }
}

const QMetaObject QImap::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QImap.data,
      qt_meta_data_QImap,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *QImap::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QImap::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QImap.stringdata0))
        return static_cast<void*>(const_cast< QImap*>(this));
    return QObject::qt_metacast(_clname);
}

int QImap::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void QImap::signalCapalibily(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QImap::signalLogin(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QImap::signalLogout()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void QImap::signalList(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void QImap::signalStatus(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void QImap::signalSelect(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void QImap::signalSearch(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void QImap::signalExamine(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void QImap::signalFetch(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void QImap::signalUidSearch(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void QImap::signalConnected(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void QImap::signalUnkown(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
