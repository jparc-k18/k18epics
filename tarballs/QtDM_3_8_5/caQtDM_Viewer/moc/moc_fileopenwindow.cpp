/****************************************************************************
** Meta object code from reading C++ file 'fileopenwindow.h'
**
** Created: Thu Dec 4 10:19:51 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/fileopenwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'fileopenwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_FileOpenWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      24,   16,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      50,   15,   15,   15, 0x08,
      73,   15,   15,   15, 0x08,
      97,   15,   15,   15, 0x08,
     119,   15,   15,   15, 0x08,
     142,   15,   15,   15, 0x08,
     164,   15,   15,   15, 0x08,
     186,   15,   15,   15, 0x08,
     210,   15,   15,   15, 0x08,
     242,  239,   15,   15, 0x08,
     288,   15,   15,   15, 0x08,
     306,   15,   15,   15, 0x08,
     334,  330,   15,   15, 0x08,
     371,   15,   15,   15, 0x0a,
     385,   15,   15,   15, 0x0a,
     398,   15,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_FileOpenWindow[] = {
    "FileOpenWindow\0\0message\0"
    "messageAvailable(QString)\0"
    "Callback_ActionTimed()\0Callback_ActionDirect()\0"
    "Callback_OpenButton()\0Callback_ActionAbout()\0"
    "Callback_ActionExit()\0Callback_ActionHelp()\0"
    "Callback_ActionReload()\0"
    "Callback_ActionUnconnected()\0,,\0"
    "Callback_OpenNewFile(QString,QString,QString)\0"
    "checkForMessage()\0Callback_PVwindowExit()\0"
    "err\0processError(QProcess::ProcessError)\0"
    "doSomething()\0nextWindow()\0"
    "Callback_IosExit()\0"
};

void FileOpenWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        FileOpenWindow *_t = static_cast<FileOpenWindow *>(_o);
        switch (_id) {
        case 0: _t->messageAvailable((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->Callback_ActionTimed(); break;
        case 2: _t->Callback_ActionDirect(); break;
        case 3: _t->Callback_OpenButton(); break;
        case 4: _t->Callback_ActionAbout(); break;
        case 5: _t->Callback_ActionExit(); break;
        case 6: _t->Callback_ActionHelp(); break;
        case 7: _t->Callback_ActionReload(); break;
        case 8: _t->Callback_ActionUnconnected(); break;
        case 9: _t->Callback_OpenNewFile((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 10: _t->checkForMessage(); break;
        case 11: _t->Callback_PVwindowExit(); break;
        case 12: _t->processError((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        case 13: _t->doSomething(); break;
        case 14: _t->nextWindow(); break;
        case 15: _t->Callback_IosExit(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData FileOpenWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject FileOpenWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FileOpenWindow,
      qt_meta_data_FileOpenWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &FileOpenWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *FileOpenWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *FileOpenWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FileOpenWindow))
        return static_cast<void*>(const_cast< FileOpenWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int FileOpenWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void FileOpenWindow::messageAvailable(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
