/****************************************************************************
** Meta object code from reading C++ file 'smtp2.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../5edmet louled/smtp2.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'smtp2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Smtp2_t {
    QByteArrayData data[12];
    char stringdata0[156];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Smtp2_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Smtp2_t qt_meta_stringdata_Smtp2 = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Smtp2"
QT_MOC_LITERAL(1, 6, 6), // "status"
QT_MOC_LITERAL(2, 13, 0), // ""
QT_MOC_LITERAL(3, 14, 12), // "stateChanged"
QT_MOC_LITERAL(4, 27, 28), // "QAbstractSocket::SocketState"
QT_MOC_LITERAL(5, 56, 11), // "socketState"
QT_MOC_LITERAL(6, 68, 13), // "errorReceived"
QT_MOC_LITERAL(7, 82, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(8, 111, 11), // "socketError"
QT_MOC_LITERAL(9, 123, 12), // "disconnected"
QT_MOC_LITERAL(10, 136, 9), // "connected"
QT_MOC_LITERAL(11, 146, 9) // "readyRead"

    },
    "Smtp2\0status\0\0stateChanged\0"
    "QAbstractSocket::SocketState\0socketState\0"
    "errorReceived\0QAbstractSocket::SocketError\0"
    "socketError\0disconnected\0connected\0"
    "readyRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Smtp2[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   47,    2, 0x08 /* Private */,
       6,    1,   50,    2, 0x08 /* Private */,
       9,    0,   53,    2, 0x08 /* Private */,
      10,    0,   54,    2, 0x08 /* Private */,
      11,    0,   55,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Smtp2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Smtp2 *_t = static_cast<Smtp2 *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->status((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->stateChanged((*reinterpret_cast< QAbstractSocket::SocketState(*)>(_a[1]))); break;
        case 2: _t->errorReceived((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: _t->disconnected(); break;
        case 4: _t->connected(); break;
        case 5: _t->readyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketState >(); break;
            }
            break;
        case 2:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Smtp2::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Smtp2::status)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Smtp2::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Smtp2.data,
      qt_meta_data_Smtp2,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Smtp2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Smtp2::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Smtp2.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Smtp2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void Smtp2::status(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
