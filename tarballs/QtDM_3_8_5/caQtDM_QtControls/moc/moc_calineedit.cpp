/****************************************************************************
** Meta object code from reading C++ file 'calineedit.h'
**
** Created: Thu Dec 4 10:19:09 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/calineedit.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'calineedit.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_caLineEdit[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
      19,   19, // properties
       5,   76, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      20,   12,   11,   11, 0x08,

 // properties: name, type, flags
      49,   41, 0x0a095003,
      64,   57, 0x43095103,
      75,   57, 0x43095103,
      94,   86, 0x0009510b,
     109,  104, 0x01094103,
     115,  104, 0x01095003,
     128,   57, 0x43095103,
     143,  139, 0x02095003,
     172,  158, 0x0009510b,
     186,  139, 0x02095103,
     207,  196, 0x0009510b,
     221,  196, 0x0009510b,
     239,  232, 0x06095103,
     248,  232, 0x06095103,
     257,  104, 0x01094001,
     284,  274, 0x0009510b,
     298,  232, 0x06094103,
     314,  104, 0x01095103,
     338,  327, 0x0009510b,

 // enums: name, flags, count, data
      86, 0x0,    4,   96,
     158, 0x0,    2,  104,
     196, 0x0,    2,  108,
     327, 0x0,   11,  112,
     274, 0x0,    3,  134,

 // enum data: key, value
     349, uint(caLineEdit::Default),
     357, uint(caLineEdit::Static),
     364, uint(caLineEdit::Alarm_Default),
     378, uint(caLineEdit::Alarm_Static),
     391, uint(caLineEdit::onForeground),
     404, uint(caLineEdit::onBackground),
     417, uint(caLineEdit::Channel),
     425, uint(caLineEdit::User),
     430, uint(caLineEdit::decimal),
     438, uint(caLineEdit::exponential),
     450, uint(caLineEdit::engr_notation),
     464, uint(caLineEdit::compact),
     472, uint(caLineEdit::truncated),
     482, uint(caLineEdit::hexadecimal),
     494, uint(caLineEdit::octal),
     500, uint(caLineEdit::string),
     507, uint(caLineEdit::sexagesimal),
     519, uint(caLineEdit::sexagesimal_hms),
     535, uint(caLineEdit::sexagesimal_dms),
     551, uint(caLineEdit::None),
     556, uint(caLineEdit::Height),
     563, uint(caLineEdit::WidthAndHeight),

       0        // eod
};

static const char qt_meta_stringdata_caLineEdit[] = {
    "caLineEdit\0\0newText\0rescaleFont(QString)\0"
    "QString\0channel\0QColor\0foreground\0"
    "background\0colMode\0colorMode\0bool\0"
    "frame\0framePresent\0frameColor\0int\0"
    "frameLineWidth\0alertHandling\0alarmHandling\0"
    "precision\0SourceMode\0precisionMode\0"
    "limitsMode\0double\0maxValue\0minValue\0"
    "fontScaleEnabled\0ScaleMode\0fontScaleMode\0"
    "fontScaleFactor\0unitsEnabled\0FormatType\0"
    "formatType\0Default\0Static\0Alarm_Default\0"
    "Alarm_Static\0onForeground\0onBackground\0"
    "Channel\0User\0decimal\0exponential\0"
    "engr_notation\0compact\0truncated\0"
    "hexadecimal\0octal\0string\0sexagesimal\0"
    "sexagesimal_hms\0sexagesimal_dms\0None\0"
    "Height\0WidthAndHeight\0"
};

void caLineEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        caLineEdit *_t = static_cast<caLineEdit *>(_o);
        switch (_id) {
        case 0: _t->rescaleFont((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData caLineEdit::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject caLineEdit::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_caLineEdit,
      qt_meta_data_caLineEdit, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &caLineEdit::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *caLineEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *caLineEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_caLineEdit))
        return static_cast<void*>(const_cast< caLineEdit*>(this));
    if (!strcmp(_clname, "FontScalingWidget"))
        return static_cast< FontScalingWidget*>(const_cast< caLineEdit*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int caLineEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
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
        case 0: *reinterpret_cast< QString*>(_v) = getPV(); break;
        case 1: *reinterpret_cast< QColor*>(_v) = getForeground(); break;
        case 2: *reinterpret_cast< QColor*>(_v) = getBackground(); break;
        case 3: *reinterpret_cast< colMode*>(_v) = getColorMode(); break;
        case 4: *reinterpret_cast< bool*>(_v) = getFrame(); break;
        case 5: *reinterpret_cast< bool*>(_v) = getFrame(); break;
        case 6: *reinterpret_cast< QColor*>(_v) = getFrameColor(); break;
        case 7: *reinterpret_cast< int*>(_v) = getLineWidth(); break;
        case 8: *reinterpret_cast< alertHandling*>(_v) = getAlarmHandling(); break;
        case 9: *reinterpret_cast< int*>(_v) = getPrecision(); break;
        case 10: *reinterpret_cast< SourceMode*>(_v) = getPrecisionMode(); break;
        case 11: *reinterpret_cast< SourceMode*>(_v) = getLimitsMode(); break;
        case 12: *reinterpret_cast< double*>(_v) = getMaxValue(); break;
        case 13: *reinterpret_cast< double*>(_v) = getMinValue(); break;
        case 14: *reinterpret_cast< bool*>(_v) = fontScaleEnabled(); break;
        case 15: *reinterpret_cast< ScaleMode*>(_v) = fontScaleMode(); break;
        case 16: *reinterpret_cast< double*>(_v) = fontScaleFactor(); break;
        case 17: *reinterpret_cast< bool*>(_v) = getUnitsEnabled(); break;
        case 18: *reinterpret_cast< FormatType*>(_v) = getFormatType(); break;
        }
        _id -= 19;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setPV(*reinterpret_cast< QString*>(_v)); break;
        case 1: setForeground(*reinterpret_cast< QColor*>(_v)); break;
        case 2: setBackground(*reinterpret_cast< QColor*>(_v)); break;
        case 3: setColorMode(*reinterpret_cast< colMode*>(_v)); break;
        case 4: setFrame(*reinterpret_cast< bool*>(_v)); break;
        case 5: setFrame(*reinterpret_cast< bool*>(_v)); break;
        case 6: setFrameColor(*reinterpret_cast< QColor*>(_v)); break;
        case 7: setLinewidth(*reinterpret_cast< int*>(_v)); break;
        case 8: setAlarmHandling(*reinterpret_cast< alertHandling*>(_v)); break;
        case 9: setPrecision(*reinterpret_cast< int*>(_v)); break;
        case 10: setPrecisionMode(*reinterpret_cast< SourceMode*>(_v)); break;
        case 11: setLimitsMode(*reinterpret_cast< SourceMode*>(_v)); break;
        case 12: setMaxValue(*reinterpret_cast< double*>(_v)); break;
        case 13: setMinValue(*reinterpret_cast< double*>(_v)); break;
        case 15: setFontScaleMode(*reinterpret_cast< ScaleMode*>(_v)); break;
        case 16: setFontScaleFactor(*reinterpret_cast< double*>(_v)); break;
        case 17: setUnitsEnabled(*reinterpret_cast< bool*>(_v)); break;
        case 18: setFormatType(*reinterpret_cast< FormatType*>(_v)); break;
        }
        _id -= 19;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 19;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 19;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 19;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 19;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 19;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 19;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
