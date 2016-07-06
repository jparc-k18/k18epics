/****************************************************************************
** Meta object code from reading C++ file 'caled.h'
**
** Created: Thu Dec 4 10:19:10 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/caled.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'caled.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_caLed[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       7,   14, // properties
       1,   35, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      14,    6, 0x0a095003,
      30,   22, 0x0009510b,
      47,   40, 0x43095103,
      58,   40, 0x43095103,
      68,   40, 0x43095103,
      83,    6, 0x0a095103,
      93,    6, 0x0a095103,

 // enums: name, flags, count, data
      22, 0x0,    2,   39,

 // enum data: key, value
     104, uint(caLed::Static),
     111, uint(caLed::Alarm),

       0        // eod
};

static const char qt_meta_stringdata_caLed[] = {
    "caLed\0QString\0channel\0colMode\0colorMode\0"
    "QColor\0falseColor\0trueColor\0undefinedColor\0"
    "trueValue\0falseValue\0Static\0Alarm\0"
};

void caLed::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData caLed::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject caLed::staticMetaObject = {
    { &ELed::staticMetaObject, qt_meta_stringdata_caLed,
      qt_meta_data_caLed, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &caLed::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *caLed::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *caLed::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_caLed))
        return static_cast<void*>(const_cast< caLed*>(this));
    return ELed::qt_metacast(_clname);
}

int caLed::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = ELed::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = getPV(); break;
        case 1: *reinterpret_cast< colMode*>(_v) = getColorMode(); break;
        case 2: *reinterpret_cast< QColor*>(_v) = getFalseColor(); break;
        case 3: *reinterpret_cast< QColor*>(_v) = getTrueColor(); break;
        case 4: *reinterpret_cast< QColor*>(_v) = getUndefinedColor(); break;
        case 5: *reinterpret_cast< QString*>(_v) = getTrueValue(); break;
        case 6: *reinterpret_cast< QString*>(_v) = getFalseValue(); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPV(*reinterpret_cast< QString*>(_v)); break;
        case 1: setColorMode(*reinterpret_cast< colMode*>(_v)); break;
        case 2: setFalseColor(*reinterpret_cast< QColor*>(_v)); break;
        case 3: setTrueColor(*reinterpret_cast< QColor*>(_v)); break;
        case 4: setUndefinedColor(*reinterpret_cast< QColor*>(_v)); break;
        case 5: setTrueValue(*reinterpret_cast< QString*>(_v)); break;
        case 6: setFalseValue(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 7;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 7;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 7;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
