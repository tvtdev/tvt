/****************************************************************************
** Meta object code from reading C++ file 'httplib.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../httplib.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'httplib.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HttpLib_t {
    QByteArrayData data[39];
    char stringdata0[454];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HttpLib_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HttpLib_t qt_meta_stringdata_HttpLib = {
    {
QT_MOC_LITERAL(0, 0, 7), // "HttpLib"
QT_MOC_LITERAL(1, 8, 13), // "signalRawData"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 4), // "data"
QT_MOC_LITERAL(4, 28, 10), // "httpStatus"
QT_MOC_LITERAL(5, 39, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(6, 67, 5), // "error"
QT_MOC_LITERAL(7, 73, 14), // "signalJsonData"
QT_MOC_LITERAL(8, 88, 4), // "json"
QT_MOC_LITERAL(9, 93, 16), // "signalStringData"
QT_MOC_LITERAL(10, 110, 22), // "signalDownloadFinished"
QT_MOC_LITERAL(11, 133, 8), // "filePath"
QT_MOC_LITERAL(12, 142, 14), // "signalFinished"
QT_MOC_LITERAL(13, 157, 3), // "get"
QT_MOC_LITERAL(14, 161, 3), // "url"
QT_MOC_LITERAL(15, 165, 9), // "QUrlQuery"
QT_MOC_LITERAL(16, 175, 6), // "params"
QT_MOC_LITERAL(17, 182, 21), // "QMap<QString,QString>"
QT_MOC_LITERAL(18, 204, 4), // "post"
QT_MOC_LITERAL(19, 209, 10), // "uploadFile"
QT_MOC_LITERAL(20, 220, 8), // "fileName"
QT_MOC_LITERAL(21, 229, 12), // "downloadFile"
QT_MOC_LITERAL(22, 242, 9), // "savedPath"
QT_MOC_LITERAL(23, 252, 4), // "code"
QT_MOC_LITERAL(24, 257, 9), // "sslErrors"
QT_MOC_LITERAL(25, 267, 16), // "QList<QSslError>"
QT_MOC_LITERAL(26, 284, 6), // "errors"
QT_MOC_LITERAL(27, 291, 16), // "sslManagerErrors"
QT_MOC_LITERAL(28, 308, 14), // "QNetworkReply*"
QT_MOC_LITERAL(29, 323, 5), // "reply"
QT_MOC_LITERAL(30, 329, 8), // "finished"
QT_MOC_LITERAL(31, 338, 9), // "readReady"
QT_MOC_LITERAL(32, 348, 14), // "uploadProgress"
QT_MOC_LITERAL(33, 363, 9), // "bytesSent"
QT_MOC_LITERAL(34, 373, 10), // "bytesTotal"
QT_MOC_LITERAL(35, 384, 16), // "downloadProgress"
QT_MOC_LITERAL(36, 401, 13), // "bytesReceived"
QT_MOC_LITERAL(37, 415, 17), // "downloadReadReady"
QT_MOC_LITERAL(38, 433, 20) // "downloadFileFinished"

    },
    "HttpLib\0signalRawData\0\0data\0httpStatus\0"
    "QNetworkReply::NetworkError\0error\0"
    "signalJsonData\0json\0signalStringData\0"
    "signalDownloadFinished\0filePath\0"
    "signalFinished\0get\0url\0QUrlQuery\0"
    "params\0QMap<QString,QString>\0post\0"
    "uploadFile\0fileName\0downloadFile\0"
    "savedPath\0code\0sslErrors\0QList<QSslError>\0"
    "errors\0sslManagerErrors\0QNetworkReply*\0"
    "reply\0finished\0readReady\0uploadProgress\0"
    "bytesSent\0bytesTotal\0downloadProgress\0"
    "bytesReceived\0downloadReadReady\0"
    "downloadFileFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HttpLib[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      25,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,  139,    2, 0x06 /* Public */,
       7,    3,  146,    2, 0x06 /* Public */,
       9,    3,  153,    2, 0x06 /* Public */,
      10,    3,  160,    2, 0x06 /* Public */,
      12,    0,  167,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    2,  168,    2, 0x0a /* Public */,
      13,    1,  173,    2, 0x2a /* Public | MethodCloned */,
      13,    2,  176,    2, 0x0a /* Public */,
      18,    2,  181,    2, 0x0a /* Public */,
      18,    1,  186,    2, 0x2a /* Public | MethodCloned */,
      18,    2,  189,    2, 0x0a /* Public */,
      18,    2,  194,    2, 0x0a /* Public */,
      18,    2,  199,    2, 0x0a /* Public */,
      19,    3,  204,    2, 0x0a /* Public */,
      19,    2,  211,    2, 0x2a /* Public | MethodCloned */,
      21,    2,  216,    2, 0x0a /* Public */,
       6,    1,  221,    2, 0x08 /* Private */,
      24,    1,  224,    2, 0x08 /* Private */,
      27,    2,  227,    2, 0x08 /* Private */,
      30,    0,  232,    2, 0x08 /* Private */,
      31,    0,  233,    2, 0x08 /* Private */,
      32,    2,  234,    2, 0x08 /* Private */,
      35,    2,  239,    2, 0x08 /* Private */,
      37,    0,  244,    2, 0x08 /* Private */,
      38,    0,  245,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QByteArray, QMetaType::Int, 0x80000000 | 5,    3,    4,    6,
    QMetaType::Void, QMetaType::QJsonDocument, QMetaType::Int, 0x80000000 | 5,    8,    4,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, 0x80000000 | 5,    3,    4,    6,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, 0x80000000 | 5,   11,    4,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Bool, QMetaType::QUrl, 0x80000000 | 15,   14,   16,
    QMetaType::Bool, QMetaType::QUrl,   14,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 17,   14,   16,
    QMetaType::Bool, QMetaType::QUrl, 0x80000000 | 15,   14,   16,
    QMetaType::Bool, QMetaType::QUrl,   14,
    QMetaType::Bool, QMetaType::QString, 0x80000000 | 17,   14,   16,
    QMetaType::Bool, QMetaType::QUrl, QMetaType::QByteArray,   14,    3,
    QMetaType::Bool, QMetaType::QString, QMetaType::QByteArray,   14,    3,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString, QMetaType::QString,   14,   11,   20,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   14,   11,
    QMetaType::Bool, QMetaType::QString, QMetaType::QString,   14,   22,
    QMetaType::Void, 0x80000000 | 5,   23,
    QMetaType::Void, 0x80000000 | 25,   26,
    QMetaType::Void, 0x80000000 | 28, 0x80000000 | 25,   29,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,   33,   34,
    QMetaType::Void, QMetaType::LongLong, QMetaType::LongLong,   36,   34,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HttpLib::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HttpLib *_t = static_cast<HttpLib *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalRawData((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[3]))); break;
        case 1: _t->signalJsonData((*reinterpret_cast< QJsonDocument(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[3]))); break;
        case 2: _t->signalStringData((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[3]))); break;
        case 3: _t->signalDownloadFinished((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[3]))); break;
        case 4: _t->signalFinished(); break;
        case 5: { bool _r = _t->get((*reinterpret_cast< QUrl(*)>(_a[1])),(*reinterpret_cast< QUrlQuery(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->get((*reinterpret_cast< QUrl(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->get((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QMap<QString,QString>(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { bool _r = _t->post((*reinterpret_cast< QUrl(*)>(_a[1])),(*reinterpret_cast< QUrlQuery(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { bool _r = _t->post((*reinterpret_cast< QUrl(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { bool _r = _t->post((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QMap<QString,QString>(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 11: { bool _r = _t->post((*reinterpret_cast< QUrl(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 12: { bool _r = _t->post((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QByteArray(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 13: { bool _r = _t->uploadFile((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 14: { bool _r = _t->uploadFile((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 15: { bool _r = _t->downloadFile((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 16: _t->error((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 17: _t->sslErrors((*reinterpret_cast< const QList<QSslError>(*)>(_a[1]))); break;
        case 18: _t->sslManagerErrors((*reinterpret_cast< QNetworkReply*(*)>(_a[1])),(*reinterpret_cast< QList<QSslError>(*)>(_a[2]))); break;
        case 19: _t->finished(); break;
        case 20: _t->readReady(); break;
        case 21: _t->uploadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 22: _t->downloadProgress((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2]))); break;
        case 23: _t->downloadReadReady(); break;
        case 24: _t->downloadFileFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 2:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        case 16:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QSslError> >(); break;
            }
            break;
        case 18:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QList<QSslError> >(); break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (HttpLib::*_t)(QByteArray , int , QNetworkReply::NetworkError );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HttpLib::signalRawData)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (HttpLib::*_t)(QJsonDocument , int , QNetworkReply::NetworkError );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HttpLib::signalJsonData)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (HttpLib::*_t)(QString , int , QNetworkReply::NetworkError );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HttpLib::signalStringData)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (HttpLib::*_t)(QString , int , QNetworkReply::NetworkError );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HttpLib::signalDownloadFinished)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (HttpLib::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&HttpLib::signalFinished)) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject HttpLib::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_HttpLib.data,
      qt_meta_data_HttpLib,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *HttpLib::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HttpLib::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_HttpLib.stringdata0))
        return static_cast<void*>(const_cast< HttpLib*>(this));
    return QObject::qt_metacast(_clname);
}

int HttpLib::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 25)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 25;
    }
    return _id;
}

// SIGNAL 0
void HttpLib::signalRawData(QByteArray _t1, int _t2, QNetworkReply::NetworkError _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HttpLib::signalJsonData(QJsonDocument _t1, int _t2, QNetworkReply::NetworkError _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HttpLib::signalStringData(QString _t1, int _t2, QNetworkReply::NetworkError _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void HttpLib::signalDownloadFinished(QString _t1, int _t2, QNetworkReply::NetworkError _t3)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void HttpLib::signalFinished()
{
    QMetaObject::activate(this, &staticMetaObject, 4, Q_NULLPTR);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
