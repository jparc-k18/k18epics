/****************************************************************************
** Meta object code from reading C++ file 'egauge.h'
**
** Created: Thu Dec 4 10:19:13 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/egauge.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'egauge.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_EAbstractGauge[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
      18,   24, // properties
       3,   78, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x0a,
      33,   15,   15,   15, 0x0a,

 // properties: name, type, flags
      54,   47, 0x06095103,
      63,   47, 0x06095103,
      84,   72, 0x0009510b,
      98,   47, 0x06095103,
     108,   47, 0x06095103,
     117,   47, 0x06095103,
     128,   47, 0x06095103,
     150,  140, 0x0009510b,
     162,   47, 0x06095103,
     168,   47, 0x06095103,
     183,  178, 0x01095103,
     196,  178, 0x01095103,
     223,  213, 0x0009510b,
     233,  178, 0x01095103,
     247,  178, 0x01095103,
     272,  264, 0x0a095103,
     288,  284, 0x02095103,
     302,  284, 0x02095103,

 // enums: name, flags, count, data
      72, 0x0,    2,   90,
     140, 0x0,    3,   94,
     213, 0x0,    3,  100,

 // enum data: key, value
     316, uint(EAbstractGauge::Channel_Limits),
     331, uint(EAbstractGauge::User_Limits),
     343, uint(EAbstractGauge::Channel_Alarms),
     358, uint(EAbstractGauge::User_Alarms),
     370, uint(EAbstractGauge::None),
     375, uint(EAbstractGauge::GRADIENT),
     384, uint(EAbstractGauge::COLORBAR),
     393, uint(EAbstractGauge::SINGLECOLOR),

       0        // eod
};

static const char qt_meta_stringdata_EAbstractGauge[] = {
    "EAbstractGauge\0\0setValue(double)\0"
    "setValue(int)\0double\0minValue\0maxValue\0"
    "displayLims\0displayLimits\0highError\0"
    "lowError\0lowWarning\0highWarning\0"
    "alarmLims\0alarmLimits\0value\0reference\0"
    "bool\0scaleEnabled\0referenceEnabled\0"
    "ColorMode\0colorMode\0externalScale\0"
    "logarithmicScale\0QString\0valueFormat\0"
    "int\0numMajorTicks\0numMinorTicks\0"
    "Channel_Limits\0User_Limits\0Channel_Alarms\0"
    "User_Alarms\0None\0GRADIENT\0COLORBAR\0"
    "SINGLECOLOR\0"
};

void EAbstractGauge::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        EAbstractGauge *_t = static_cast<EAbstractGauge *>(_o);
        switch (_id) {
        case 0: _t->setValue((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->setValue((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData EAbstractGauge::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject EAbstractGauge::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_EAbstractGauge,
      qt_meta_data_EAbstractGauge, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &EAbstractGauge::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *EAbstractGauge::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *EAbstractGauge::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_EAbstractGauge))
        return static_cast<void*>(const_cast< EAbstractGauge*>(this));
    return QWidget::qt_metacast(_clname);
}

int EAbstractGauge::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = minValue(); break;
        case 1: *reinterpret_cast< double*>(_v) = maxValue(); break;
        case 2: *reinterpret_cast< displayLims*>(_v) = getDisplayLimits(); break;
        case 3: *reinterpret_cast< double*>(_v) = highError(); break;
        case 4: *reinterpret_cast< double*>(_v) = lowError(); break;
        case 5: *reinterpret_cast< double*>(_v) = lowWarning(); break;
        case 6: *reinterpret_cast< double*>(_v) = highWarning(); break;
        case 7: *reinterpret_cast< alarmLims*>(_v) = getAlarmLimits(); break;
        case 8: *reinterpret_cast< double*>(_v) = value(); break;
        case 9: *reinterpret_cast< double*>(_v) = reference(); break;
        case 10: *reinterpret_cast< bool*>(_v) = isScaleEnabled(); break;
        case 11: *reinterpret_cast< bool*>(_v) = isReferenceEnabled(); break;
        case 12: *reinterpret_cast< ColorMode*>(_v) = colorMode(); break;
        case 13: *reinterpret_cast< bool*>(_v) = externalScale(); break;
        case 14: *reinterpret_cast< bool*>(_v) = logarithmicScale(); break;
        case 15: *reinterpret_cast< QString*>(_v) = valueFormat(); break;
        case 16: *reinterpret_cast< int*>(_v) = numMajorTicks(); break;
        case 17: *reinterpret_cast< int*>(_v) = numMinorTicks(); break;
        }
        _id -= 18;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setMinValue(*reinterpret_cast< double*>(_v)); break;
        case 1: setMaxValue(*reinterpret_cast< double*>(_v)); break;
        case 2: setDisplayLimits(*reinterpret_cast< displayLims*>(_v)); break;
        case 3: setHighError(*reinterpret_cast< double*>(_v)); break;
        case 4: setLowError(*reinterpret_cast< double*>(_v)); break;
        case 5: setLowWarning(*reinterpret_cast< double*>(_v)); break;
        case 6: setHighWarning(*reinterpret_cast< double*>(_v)); break;
        case 7: setAlarmLimits(*reinterpret_cast< alarmLims*>(_v)); break;
        case 8: setValue(*reinterpret_cast< double*>(_v)); break;
        case 9: setReference(*reinterpret_cast< double*>(_v)); break;
        case 10: setScaleEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 11: setReferenceEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 12: setColorMode(*reinterpret_cast< ColorMode*>(_v)); break;
        case 13: setExternalScale(*reinterpret_cast< bool*>(_v)); break;
        case 14: setLogarithmicScale(*reinterpret_cast< bool*>(_v)); break;
        case 15: setValueFormat(*reinterpret_cast< QString*>(_v)); break;
        case 16: setNumMajorTicks(*reinterpret_cast< int*>(_v)); break;
        case 17: setNumMinorTicks(*reinterpret_cast< int*>(_v)); break;
        }
        _id -= 18;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 18;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 18;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
