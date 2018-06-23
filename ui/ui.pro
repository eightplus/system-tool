TARGET = system-tool
TEMPLATE = app

QT += widgets gui dbus

CONFIG += c++11 link_pkgconfig
PKGCONFIG += libgtop-2.0

#QMAKE_CXXFLAGS += -fpermissive
QMAKE_CPPFLAGS *= $(shell dpkg-buildflags --get CPPFLAGS)
QMAKE_CFLAGS   *= $(shell dpkg-buildflags --get CFLAGS)
QMAKE_CXXFLAGS *= $(shell dpkg-buildflags --get CXXFLAGS)
QMAKE_LFLAGS   *= $(shell dpkg-buildflags --get LDFLAGS)

#CONFIG += link_pkgconfig
#PKGCONFIG += glib-2.0 gobject-2.0
#LIBS += -ldl -lpthread

inst1.files += res/system-tool.png
inst1.path = /usr/share/pixmaps
inst2.files += ../system-tool.desktop
inst2.path = /usr/share/applications
target.source  += $$TARGET
target.path = /usr/bin
#INSTALLS += target
INSTALLS += inst1 \
    inst2 \
    target


unix {
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

HEADERS += \
    maindialog.h \
    basewidget.h \
    infowidget.h \
    filesyswidget.h \
    filesystemlistwidget.h \
    filesystemlistitem.h \
    filesystemworker.h \
    filesystemdata.h \
    cpuwidget.h \
    cpuratewidget.h \
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
    filesyswidget.cpp \
    filesystemlistwidget.cpp \
    filesystemlistitem.cpp \
    filesystemworker.cpp \
    filesystemdata.cpp \
    cpuwidget.cpp \
    cpuratewidget.cpp \
    mywidget.cpp \
    myworker.cpp \
    myitem.cpp \
    mygroup.cpp

RESOURCES += \
    res.qrc

