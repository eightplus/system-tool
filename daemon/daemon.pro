TARGET = system-tool-daemon
TEMPLATE = app

QT += core dbus

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
service.files = $$PWD/conf/org.freedesktop.systemtool.service
service.path = $${PREFIX}/share/dbus-1/system-services/

dbus.files = $$PWD/conf/org.freedesktop.systemtool.conf
dbus.path = /etc/dbus-1/system.d/

INSTALLS += service dbus
}

unix {
    MOC_DIR = .moc
    OBJECTS_DIR = .obj
}

HEADERS += \
    daemon.h \
    utils.h

SOURCES += \
    main.cpp \
    daemon.cpp \
    utils.cpp

