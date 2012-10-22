#-------------------------------------------------
#
# Project created by QtCreator 2010-11-03T21:01:40
#
#-------------------------------------------------

QT       += declarative sql xml xmlpatterns network

TARGET = omweather-qml
TEMPLATE = app


SOURCES += main.cpp \
    dataqml.cpp \
    configqml.cpp \
    qmllayoutitem.cpp \
    dataitem.cpp \
    datamodel.cpp \
    dbusadaptor.cpp \
    dbusadaptor_applet.cpp \
    controller.cpp \
    networkingcontrol.cpp \
    selectmodel.cpp \
    updatethread.cpp \
    countrymodel.cpp \
    regionmodel.cpp \
    citymodel.cpp \
    gpsposition.cpp

HEADERS  += \
    dataqml.h \
    configqml.h \
    qmllayoutitem.h \
    dataitem.h \
    datamodel.h \
    dbusadaptor.h \
    dbusadaptor_applet.h \
    controller.h \
    networkingcontrol.h \
    selectmodel.h \
    updatethread.h \
    countrymodel.h \
    regionmodel.h \
    citymodel.h \
    gpsposition.h

FORMS    +=
#RESOURCES += weatherlayoutitem.qrc

OTHER_FILES += \
    qml/layoutitem.qml \
    qml/weatherlayoutitem.qml \
    qml/Details.qml \
    qml/ImageButton.qml \
    qml/TextButton.qml \
    qml/omweather.qml \
    qml/omweatherpanel.qml \
    qml/main.qml \
    qml/WeatherPage.qml \
    qml/SettingsPage.qml \
    qml/FullWeatherPage.qml \
    qml/AboutPage.qml \
    qml/StationsPage.qml \
    qml/NewStationPage.qml \
    qml/UnitsPage.qml \
    qml/VisualsPage.qml \
    qml/ListPage.qml \
    qml/SourcePage.qml \
    qml/CountryPage.qml \
    qml/RegionPage.qml \
    qml/CityPage.qml \
    qml/IconsetPage.qml \
    qml/StandbyScreen.qml \
    qml/ColorCell.qml \
    qml/SearchField.qml

CONFIG(localdebug):DEFINES += LOCALDEBUG

CONFIG += mobility
MOBILITY += location
CONFIG += qdeclarative-boostable
CONFIG += meegotouch 

QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic


INCLUDEPATH += ../core                                                                                                        
LIBS += -L ../core ../core/libomweather-core.a  
CONFIG += qdbus
CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0
PKGCONFIG += sqlite3
PKGCONFIG += libcurl
target.path = /opt/com.meecast.omweather/bin
INSTALLS += target

desktop.files = meecast.desktop
desktop.path = /usr/share/applications
icon64.path = /usr/share/pixmaps
icon64.files += omweather.png
qml.files = qml/*.qml *.js
qml.path = /opt/com.meecast.omweather/share/omweather/qml
searchicon.files += gfx/*.png
searchicon.path += /opt/com.meecast.omweather/share/omweather/qml/gfx

#INSTALLS += desktop qml icon64
INSTALLS += desktop qml searchicon

DATADIR=/opt/com.meecast.omweather/share

TRANSLIST = bg_BG \
            ca_CA \
            de_DE \
            en_GB \
            en_US \
            es_ES \
            es_MX \
            fi_FI \
            fr_FR \
            it_IT \
            lv_LV \
            nl_NL \
            pl_PL \
            pt_PT \
            ru_RU \
            sv_SV \
            sk_SK \
            tr_TR \
            vi_VI \
            zh_CN \
	    zh_ZH

for(language, TRANSLIST):TRANSLATIONS += po/locale/$${language}/$${language}.po
!isEmpty(TRANSLATIONS) {
    isEmpty(QMAKE_LRELEASE) {
        win32:QMAKE_LRELEASE = msgfmt.exe
        else:QMAKE_LRELEASE = msgfmt
    }
    TSQM.name = msgfmt \
        ${QMAKE_FILE_IN}
    TSQM.input = TRANSLATIONS
    TSQM.output = ${QMAKE_FILE_PATH}/omweather.mo
    TSQM.commands = $$QMAKE_LRELEASE \
        -o \
        $$TSQM.output \
        ${QMAKE_FILE_IN}
    TSQM.CONFIG = no_link
    QMAKE_EXTRA_COMPILERS += TSQM
    PRE_TARGETDEPS += compiler_TSQM_make_all
}
else:message(No translation files in project)

#install

transinstallbg.extra = cp po/locale/bg_BG/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/bg/LC_MESSAGES
transinstallbg.path = /opt/com.meecast.omweather/share/locale/bg/LC_MESSAGES
transinstallbg.files = po/locale/bg_BG/omweather.mo
INSTALLS += transinstallbg

transinstallca.extra = cp po/locale/ca_CA/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/ca/LC_MESSAGES
transinstallca.path = /opt/com.meecast.omweather/share/locale/ca/LC_MESSAGES
transinstallca.files = po/locale/ca_CA/omweather.mo
INSTALLS += transinstallca

transinstallde.extra = cp po/locale/de_DE/omweather.mo  $(INSTALL_ROOT)$$DATADIR/locale/de/LC_MESSAGES
transinstallde.path = /opt/com.meecast.omweather/share/locale/de/LC_MESSAGES
transinstallde.files = po/locale/de_DE/omweather.mo
INSTALLS += transinstallde

transinstallenbg.extra = cp po/locale/en_GB/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/en_GB/LC_MESSAGES
transinstallengb.path = /opt/com.meecast.omweather/share/locale/en_GB/LC_MESSAGES
transinstallengb.files = po/locale/en_GB/omweather.mo
INSTALLS += transinstallengb

transinstallenus.extra = cp po/locale/en_US/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/en_US/LC_MESSAGES
transinstallenus.path = /opt/com.meecast.omweather/share/locale/en_US/LC_MESSAGES
transinstallenus.files = po/locale/en_US/omweather.mo
INSTALLS += transinstallenus

transinstalleses.extra = cp po/locale/es_ES/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/es_ES/LC_MESSAGES
transinstalleses.path = /opt/com.meecast.omweather/share/locale/es_ES/LC_MESSAGES
transinstalleses.files = po/locale/es_ES/omweather.mo
INSTALLS += transinstalleses

transinstallesmx.extra = cp po/locale/es_MX/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/es_MX/LC_MESSAGES
transinstallesmx.path = /opt/com.meecast.omweather/share/locale/es_MX/LC_MESSAGES
transinstallesmx.files = po/locale/es_MX/omweather.mo
INSTALLS += transinstallesmx

transinstallfi.extra = cp po/locale/fi_FI/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/fi/LC_MESSAGES
transinstallfi.path = /opt/com.meecast.omweather/share/locale/fi/LC_MESSAGES
transinstallfi.files = po/locale/fi_FI/omweather.mo
INSTALLS += transinstallfi

transinstallfr.extra = cp po/locale/fr_FR/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/fr/LC_MESSAGES
transinstallfr.path = /opt/com.meecast.omweather/share/locale/fr/LC_MESSAGES
transinstallfr.files = po/locale/fr_FR/omweather.mo
INSTALLS += transinstallfr

transinstallit.extra = cp po/locale/it_IT/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/it/LC_MESSAGES
transinstallit.path = /opt/com.meecast.omweather/share/locale/it/LC_MESSAGES
transinstallit.files = po/locale/it_IT/omweather.mo
INSTALLS += transinstallit

transinstalllv.extra = cp po/locale/lv_LV/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/lv/LC_MESSAGES
transinstalllv.path = /opt/com.meecast.omweather/share/locale/lv/LC_MESSAGES
transinstalllv.files = po/locale/lv_LV/omweather.mo
INSTALLS += transinstalllv

transinstallnl.extra = cp po/locale/nl_NL/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/nl/LC_MESSAGES
transinstallnl.path = /opt/com.meecast.omweather/share/locale/nl/LC_MESSAGES
transinstallnl.files = po/locale/nl_NL/omweather.mo
INSTALLS += transinstallnl

transinstallpl.extra = cp po/locale/pl_PL/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/pl/LC_MESSAGES
transinstallpl.path = /opt/com.meecast.omweather/share/locale/pl/LC_MESSAGES
transinstallpl.files = po/locale/pl_PL/omweather.mo
INSTALLS += transinstallpl

transinstallpt.extra = cp po/locale/pt_PT/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/pt/LC_MESSAGES
transinstallpt.path = /opt/com.meecast.omweather/share/locale/pt/LC_MESSAGES
transinstallpt.files = po/locale/pt_PT/omweather.mo
INSTALLS += transinstallpt

transinstallru.extra = cp po/locale/ru_RU/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/ru/LC_MESSAGES
transinstallru.path = /opt/com.meecast.omweather/share/locale/ru/LC_MESSAGES
transinstallru.files = po/locale/ru_RU/omweather.mo
INSTALLS += transinstallru

transinstallsk.extra = cp po/locale/sk_SK/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/sk/LC_MESSAGES
transinstallsk.path = /opt/com.meecast.omweather/share/locale/sk/LC_MESSAGES
transinstallsk.files = po/locale/sk_SK/omweather.mo
INSTALLS += transinstallsk

transinstallsv.extra = cp po/locale/sv_SV/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/sv/LC_MESSAGES
transinstallsv.path = /opt/com.meecast.omweather/share/locale/sv/LC_MESSAGES
transinstallsv.files = po/locale/sk_SV/omweather.mo
INSTALLS += transinstallsv

transinstalltr.extra = cp po/locale/tr_TR/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/tr/LC_MESSAGES
transinstalltr.path = /opt/com.meecast.omweather/share/locale/tr/LC_MESSAGES
transinstalltr.files = po/locale/tr_TR/omweather.mo
INSTALLS += transinstalltr

transinstallvi.extra = cp po/locale/vi_VI/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/vi/LC_MESSAGES
transinstallvi.path = /opt/com.meecast.omweather/share/locale/vi/LC_MESSAGES
transinstallvi.files = po/locale/vi_VI/omweather.mo
INSTALLS += transinstallvi

transinstallzhcn.extra = cp po/locale/zh_CN/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/zh_CN/LC_MESSAGES
transinstallzhcn.path = /opt/com.meecast.omweather/share/locale/zh_CN/LC_MESSAGES
transinstallzhcn.files = po/locale/zh_CN/omweather.mo
INSTALLS += transinstallzhcn

transinstallzh.extra = cp po/locale/zh_ZH/omweather.mo $(INSTALL_ROOT)$$DATADIR/locale/zh/LC_MESSAGES
transinstallzh.path = /opt/com.meecast.omweather/share/locale/zh/LC_MESSAGES
transinstallzh.files = po/locale/zh_ZH/omweather.mo
INSTALLS += transinstallzh


