/****************************************************************************
** Meta object code from reading C++ file 'cawaterfallplot.h'
**
** Created: Thu Dec 4 10:19:20 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/cawaterfallplot.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cawaterfallplot.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_caWaterfallPlot[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
      15,   19, // properties
       2,   64, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x0a,

 // properties: name, type, flags
      35,   27, 0x0a095003,
      41,   27, 0x0a095103,
      48,   27, 0x0a095103,
      55,   27, 0x0a095003,
      63,   27, 0x0a095003,
      81,   81, 0x0009510b,
      94,   87, 0x06095103,
     105,  101, 0x02095003,
     120,   87, 0x06095103,
     133,   87, 0x06095103,
     163,  146, 0x0009510b,
     183,  146, 0x0009510b,
     208,  203, 0x01095103,
     213,  203, 0x01095103,
     226,  203, 0x01095103,

 // enums: name, flags, count, data
      81, 0x0,    4,   72,
     146, 0x0,    2,   80,

 // enum data: key, value
     239, uint(caWaterfallPlot::Monitor),
     247, uint(caWaterfallPlot::Millisecond),
     259, uint(caWaterfallPlot::Second),
     266, uint(caWaterfallPlot::Minute),
     273, uint(caWaterfallPlot::Channel),
     281, uint(caWaterfallPlot::User),

       0        // eod
};

static const char qt_meta_stringdata_caWaterfallPlot[] = {
    "caWaterfallPlot\0\0TimeOut()\0QString\0"
    "Title\0TitleX\0TitleY\0channel\0"
    "countNumOrChannel\0units\0double\0period\0"
    "int\0NumberOfTraces\0IntensityMax\0"
    "IntensityMin\0intensityScaling\0"
    "IntensityScalingMax\0IntensityScalingMin\0"
    "bool\0grid\0XaxisEnabled\0YaxisEnabled\0"
    "Monitor\0Millisecond\0Second\0Minute\0"
    "Channel\0User\0"
};

void caWaterfallPlot::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        caWaterfallPlot *_t = static_cast<caWaterfallPlot *>(_o);
        switch (_id) {
        case 0: _t->TimeOut(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData caWaterfallPlot::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject caWaterfallPlot::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_caWaterfallPlot,
      qt_meta_data_caWaterfallPlot, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &caWaterfallPlot::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *caWaterfallPlot::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *caWaterfallPlot::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_caWaterfallPlot))
        return static_cast<void*>(const_cast< caWaterfallPlot*>(this));
    return QWidget::qt_metacast(_clname);
}

int caWaterfallPlot::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = getTitlePlot(); break;
        case 1: *reinterpret_cast< QString*>(_v) = getTitleX(); break;
        case 2: *reinterpret_cast< QString*>(_v) = getTitleY(); break;
        case 3: *reinterpret_cast< QString*>(_v) = getPV(); break;
        case 4: *reinterpret_cast< QString*>(_v) = getCountPV(); break;
        case 5: *reinterpret_cast< units*>(_v) = getUnits(); break;
        case 6: *reinterpret_cast< double*>(_v) = getPeriod(); break;
        case 7: *reinterpret_cast< int*>(_v) = getRows(); break;
        case 8: *reinterpret_cast< double*>(_v) = getIntensityMax(); break;
        case 9: *reinterpret_cast< double*>(_v) = getIntensityMin(); break;
        case 10: *reinterpret_cast< intensityScaling*>(_v) = getIntensityScalingMax(); break;
        case 11: *reinterpret_cast< intensityScaling*>(_v) = getIntensityScalingMin(); break;
        case 12: *reinterpret_cast< bool*>(_v) = getGrid(); break;
        case 13: *reinterpret_cast< bool*>(_v) = getXaxisEnabled(); break;
        case 14: *reinterpret_cast< bool*>(_v) = getYaxisEnabled(); break;
        }
        _id -= 15;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setTitlePlot(*reinterpret_cast< QString*>(_v)); break;
        case 1: setTitleX(*reinterpret_cast< QString*>(_v)); break;
        case 2: setTitleY(*reinterpret_cast< QString*>(_v)); break;
        case 3: setPV(*reinterpret_cast< QString*>(_v)); break;
        case 4: setCountPV(*reinterpret_cast< QString*>(_v)); break;
        case 5: setUnits(*reinterpret_cast< units*>(_v)); break;
        case 6: setPeriod(*reinterpret_cast< double*>(_v)); break;
        case 7: setRows(*reinterpret_cast< int*>(_v)); break;
        case 8: setIntensityMax(*reinterpret_cast< double*>(_v)); break;
        case 9: setIntensityMin(*reinterpret_cast< double*>(_v)); break;
        case 10: setIntensityScalingMax(*reinterpret_cast< intensityScaling*>(_v)); break;
        case 11: setIntensityScalingMin(*reinterpret_cast< intensityScaling*>(_v)); break;
        case 12: setGrid(*reinterpret_cast< bool*>(_v)); break;
        case 13: setXaxisEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 14: setYaxisEnabled(*reinterpret_cast< bool*>(_v)); break;
        }
        _id -= 15;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 15;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 15;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
