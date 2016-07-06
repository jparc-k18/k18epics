/****************************************************************************
** Meta object code from reading C++ file 'catogglebutton.h'
**
** Created: Thu Dec 4 10:19:17 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/catogglebutton.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'catogglebutton.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_caToggleButton[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       8,   29, // properties
       2,   53, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      49,   15,   15,   15, 0x08,
      73,   65,   15,   15, 0x08,

 // properties: name, type, flags
      99,   94, 0x01094103,
     116,  108, 0x0a095003,
     131,  124, 0x43095103,
     142,  124, 0x43095103,
     163,  153, 0x0009510b,
     185,  177, 0x0009510b,
     195,  108, 0x0a095103,
     205,  108, 0x0a095103,

 // enums: name, flags, count, data
     177, 0x0,    3,   61,
     153, 0x0,    3,   67,

 // enum data: key, value
     216, uint(caToggleButton::Default),
     224, uint(caToggleButton::Static),
     231, uint(caToggleButton::Alarm),
     237, uint(caToggleButton::None),
     242, uint(caToggleButton::Height),
     249, uint(caToggleButton::WidthAndHeight),

       0        // eod
};

static const char qt_meta_stringdata_caToggleButton[] = {
    "caToggleButton\0\0checked\0"
    "toggleButtonSignal(bool)\0buttonToggled()\0"
    "newText\0rescaleFont(QString)\0bool\0"
    "tristate\0QString\0channel\0QColor\0"
    "foreground\0background\0ScaleMode\0"
    "fontScaleMode\0colMode\0colorMode\0"
    "trueValue\0falseValue\0Default\0Static\0"
    "Alarm\0None\0Height\0WidthAndHeight\0"
};

void caToggleButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        caToggleButton *_t = static_cast<caToggleButton *>(_o);
        switch (_id) {
        case 0: _t->toggleButtonSignal((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->buttonToggled(); break;
        case 2: _t->rescaleFont((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData caToggleButton::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject caToggleButton::staticMetaObject = {
    { &QCheckBox::staticMetaObject, qt_meta_stringdata_caToggleButton,
      qt_meta_data_caToggleButton, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &caToggleButton::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *caToggleButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *caToggleButton::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_caToggleButton))
        return static_cast<void*>(const_cast< caToggleButton*>(this));
    if (!strcmp(_clname, "FontScalingWidget"))
        return static_cast< FontScalingWidget*>(const_cast< caToggleButton*>(this));
    return QCheckBox::qt_metacast(_clname);
}

int caToggleButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QCheckBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    }
#ifndef QT_NO_PROPERTIES
      else if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< bool*>(_v) = isTristate(); break;
        case 1: *reinterpret_cast< QString*>(_v) = getPV(); break;
        case 2: *reinterpret_cast< QColor*>(_v) = getForeground(); break;
        case 3: *reinterpret_cast< QColor*>(_v) = getBackground(); break;
        case 4: *reinterpret_cast< ScaleMode*>(_v) = fontScaleMode(); break;
        case 5: *reinterpret_cast< colMode*>(_v) = getColorMode(); break;
        case 6: *reinterpret_cast< QString*>(_v) = getTrueValue(); break;
        case 7: *reinterpret_cast< QString*>(_v) = getFalseValue(); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setTristate(*reinterpret_cast< bool*>(_v)); break;
        case 1: setPV(*reinterpret_cast< QString*>(_v)); break;
        case 2: setForeground(*reinterpret_cast< QColor*>(_v)); break;
        case 3: setBackground(*reinterpret_cast< QColor*>(_v)); break;
        case 4: setFontScaleMode(*reinterpret_cast< ScaleMode*>(_v)); break;
        case 5: setColorMode(*reinterpret_cast< colMode*>(_v)); break;
        case 6: setTrueValue(*reinterpret_cast< QString*>(_v)); break;
        case 7: setFalseValue(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 8;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 8;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 8;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void caToggleButton::toggleButtonSignal(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
