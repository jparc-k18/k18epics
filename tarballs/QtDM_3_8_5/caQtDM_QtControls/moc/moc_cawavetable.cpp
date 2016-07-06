/****************************************************************************
** Meta object code from reading C++ file 'cawavetable.h'
**
** Created: Thu Dec 4 10:19:22 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/cawavetable.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cawavetable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_caWaveTable[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       5,   14, // properties
       2,   29, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
      20,   12, 0x0a095003,
      32,   28, 0x02095103,
      43,   28, 0x02095103,
      64,   53, 0x0009510b,
      90,   78, 0x0009510b,

 // enums: name, flags, count, data
      53, 0x0,    2,   37,
      78, 0x0,    2,   41,

 // enum data: key, value
     102, uint(caWaveTable::Channel),
     110, uint(caWaveTable::User),
     115, uint(caWaveTable::Horizontal),
     126, uint(caWaveTable::Vertical),

       0        // eod
};

static const char qt_meta_stringdata_caWaveTable[] = {
    "caWaveTable\0QString\0channel\0int\0"
    "columnSize\0precision\0SourceMode\0"
    "precisionMode\0Orientation\0orientation\0"
    "Channel\0User\0Horizontal\0Vertical\0"
};

void caWaveTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObjectExtraData caWaveTable::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject caWaveTable::staticMetaObject = {
    { &QTableWidget::staticMetaObject, qt_meta_stringdata_caWaveTable,
      qt_meta_data_caWaveTable, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &caWaveTable::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *caWaveTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *caWaveTable::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_caWaveTable))
        return static_cast<void*>(const_cast< caWaveTable*>(this));
    return QTableWidget::qt_metacast(_clname);
}

int caWaveTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = getPV(); break;
        case 1: *reinterpret_cast< int*>(_v) = getColumnSize(); break;
        case 2: *reinterpret_cast< int*>(_v) = getPrecision(); break;
        case 3: *reinterpret_cast< SourceMode*>(_v) = getPrecisionMode(); break;
        case 4: *reinterpret_cast< Orientation*>(_v) = getOrientation(); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPV(*reinterpret_cast< QString*>(_v)); break;
        case 1: setColumnSize(*reinterpret_cast< int*>(_v)); break;
        case 2: setPrecision(*reinterpret_cast< int*>(_v)); break;
        case 3: setPrecisionMode(*reinterpret_cast< SourceMode*>(_v)); break;
        case 4: setOrientation(*reinterpret_cast< Orientation*>(_v)); break;
        }
        _id -= 5;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 5;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 5;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
