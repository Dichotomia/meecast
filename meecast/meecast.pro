VERSION = 3.0
TEMPLATE = subdirs
CONFIG += ordered

#SUBDIRS += core qt-qml meego-mpl
SUBDIRS += core qt-qml 
exists($$QMAKE_INCDIR_QT"/../qmsystem2/qmkeys.h"):!contains(MEEGO_EDITION,harmattan): {
    SUBDIRS += xml-qml meegotouchplugin predeamon
}


CONFIG(meegopanel):SUBDIRS += netbook-UX
CONFIG(UXpanel):SUBDIRS += meego-UX-panel 
#system(pkg-config --exists meego-panel) {
# SUBDIRS += meego-mpl
#} 


