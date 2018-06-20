TARGET = system-tool
TEMPLATE = app

QT += widgets gui dbus

CONFIG += c++11 link_pkgconfig

#QMAKE_CXXFLAGS += -fpermissive
QMAKE_CPPFLAGS *= $(shell dpkg-buildflags --get CPPFLAGS)
QMAKE_CFLAGS   *= $(shell dpkg-buildflags --get CFLAGS)
QMAKE_CXXFLAGS *= $(shell dpkg-buildflags --get CXXFLAGS)
QMAKE_LFLAGS   *= $(shell dpkg-buildflags --get LDFLAGS)

#CONFIG += link_pkgconfig
#PKGCONFIG += glib-2.0 gobject-2.0
#LIBS += -ldl -lpthread

target.source += $$TARGET
target.path = /usr/bin
INSTALLS += target

unix {
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

HEADERS += \
    maindialog.h \
    basewidget.h \
    infowidget.h \
    mywidget.h \
    myworker.h \
    myitem.h \
    mygroup.h \
    utils.h

SOURCES += \
    main.cpp \
    maindialog.cpp \
    basewidget.cpp \
    infowidget.cpp \
    mywidget.cpp \
    myworker.cpp \
    myitem.cpp \
    mygroup.cpp

RESOURCES += \
    res.qrc

