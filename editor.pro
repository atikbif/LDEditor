#-------------------------------------------------
#
# Project created by QtCreator 2018-09-17T16:27:19
#
#-------------------------------------------------

QT       += core gui network widgets svg printsupport serialport

RC_ICONS = ld.ico
VERSION = 1.0.0.0

QMAKE_TARGET_COMPANY = Modern Technical Solutions Co
QMAKE_TARGET_PRODUCT = LD Editor 1.0
QMAKE_TARGET_DESCRIPTION = PLC LD Editor
QMAKE_TARGET_COPYRIGHT = Modern Technical Solutions

TARGET = editor
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

INCLUDEPATH += Elements
INCLUDEPATH += Dialogs

SOURCES += \
    Dialogs/dialogadcconfig.cpp \
    Dialogs/dialogcycleconfig.cpp \
    Dialogs/dialogplcconfig.cpp \
    Loader/ethconfigreader.cpp \
    Loader/ethconfigreaderthread.cpp \
    Loader/ethconfigwriter.cpp \
    Loader/ethconfigwriterthread.cpp \
    Loader/ethfinderthread.cpp \
    Loader/ethloader.cpp \
    Loader/ethloaderthread.cpp \
    Loader/finderthread.cpp \
    Loader/loader.cpp \
    Loader/loaderthread.cpp \
    Loader/plcfinder.cpp \
    Modbus/dialogaddmodbusvar.cpp \
    Modbus/dialogconfigmodbuscanal.cpp \
    Modbus/dialogeditmodbusvar.cpp \
    Modbus/dialogmodbusmaster.cpp \
    Modbus/modbuscanals.cpp \
    Modbus/modbusvar.cpp \
    Modbus/modbusvarsstorage.cpp \
    SourceBuilder/adcmanager.cpp \
    checksum.cpp \
        main.cpp \
    mainwindow.cpp \
    ldscene.cpp \
    Elements\ldelement.cpp \
    Elements\opencontact.cpp \
    Elements\closedcontact.cpp \
    cmdstack.cpp \
    Elements\plustwoinputs.cpp \
    Elements\multipletwoinputs.cpp \
    Elements\horline.cpp \
    Elements\verline.cpp \
    Elements\greaterelement.cpp \
    Elements\lesselement.cpp \
    Elements\greaterorequal.cpp \
    Elements\lessorequal.cpp \
    Elements\equalelement.cpp \
    Elements\notequalelement.cpp \
    Elements\variable.cpp \
    Elements\constvar.cpp \
    Elements\relay.cpp \
    Elements\minuselement.cpp \
    Elements\divideelement.cpp \
    copybufitem.cpp \
    Elements\twoinputelement.cpp \
    Elements\rstrigger.cpp \
    Elements\srtrigger.cpp \
    Elements\delayon.cpp \
    Elements\delayoff.cpp \
    plcconfig.cpp \
    plcutils.cpp \
    plcvarcontainer.cpp \
    Dialogs\dialogldelementproperties.cpp \
    Dialogs\dialogvarconfig.cpp \
    Dialogs\dialogaddvar.cpp \
    Dialogs\dialogeditnotsystemvar.cpp \
    Dialogs\dialogeditsystemvar.cpp \
    Dialogs\dialogrenamegroup.cpp \
    Dialogs\dialogconstvarproperties.cpp \
    Dialogs\dialogaddeditcomment.cpp \
    Elements\commentelement.cpp \
    serialize.cpp \
    SourceBuilder/connectionfinder.cpp \
    SourceBuilder/vartypespecifier.cpp \
    SourceBuilder/compiler.cpp \
    Checker/ldchecker.cpp \
    rtuprotocol.cpp \
    SourceBuilder/plcparams.cpp \
    Elements/notelement.cpp \
    Elements/relayenabled.cpp \
    dialogsearch.cpp \
    SourceBuilder/finder.cpp \
    Dialogs/dialogprojectconfig.cpp \
    Elements/libraryelement.cpp \
    elementlibrary.cpp

HEADERS += \
    Dialogs/dialogadcconfig.h \
    Dialogs/dialogcycleconfig.h \
    Dialogs/dialogplcconfig.h \
    Loader/ethconfigreader.h \
    Loader/ethconfigreaderthread.h \
    Loader/ethconfigwriter.h \
    Loader/ethconfigwriterthread.h \
    Loader/ethfinderthread.h \
    Loader/ethloader.h \
    Loader/ethloaderthread.h \
    Loader/finderthread.h \
    Loader/loader.h \
    Loader/loaderthread.h \
    Loader/plcfinder.h \
    Modbus/dialogaddmodbusvar.h \
    Modbus/dialogconfigmodbuscanal.h \
    Modbus/dialogeditmodbusvar.h \
    Modbus/dialogmodbusmaster.h \
    Modbus/modbuscanals.h \
    Modbus/modbusvar.h \
    Modbus/modbusvarsstorage.h \
    SourceBuilder/adcmanager.h \
    checksum.h \
        mainwindow.h \
    mainwindow.h \
    ldscene.h \
    Elements\ldelement.h \
    Elements\opencontact.h \
    Elements\closedcontact.h \
    cmdstack.h \
    Elements\plustwoinputs.h \
    Elements\multipletwoinputs.h \
    Elements\horline.h \
    Elements\verline.h \
    Elements\greaterelement.h \
    Elements\lesselement.h \
    Elements\greaterorequal.h \
    Elements\lessorequal.h \
    Elements\equalelement.h \
    Elements\notequalelement.h \
    Elements\variable.h \
    Elements\constvar.h \
    Elements\relay.h \
    Elements\minuselement.h \
    Elements\divideelement.h \
    copybufitem.h \
    Elements\twoinputelement.h \
    Elements\rstrigger.h \
    Elements\srtrigger.h \
    Elements\delayon.h \
    Elements\delayoff.h \
    plcconfig.h \
    plcutils.h \
    plcvar.h \
    plcvarcontainer.h \
    Dialogs\dialogldelementproperties.h \
    Dialogs\dialogvarconfig.h \
    Dialogs\dialogaddvar.h \
    Dialogs\dialogeditnotsystemvar.h \
    Dialogs\dialogeditsystemvar.h \
    Dialogs\dialogrenamegroup.h \
    Dialogs\dialogconstvarproperties.h \
    Dialogs\dialogaddeditcomment.h \
    Elements\commentelement.h \
    serialize.h \
    SourceBuilder/connectionfinder.h \
    SourceBuilder/vartypespecifier.h \
    SourceBuilder/compiler.h \
    Checker/ldchecker.h \
    rtuprotocol.h \
    SourceBuilder/plcparams.h \
    Elements/notelement.h \
    Elements/relayenabled.h \
    dialogsearch.h \
    SourceBuilder/finder.h \
    Dialogs/dialogprojectconfig.h \
    portconfig.h \
    Elements/libraryelement.h \
    elementlibrary.h

FORMS += \
    Dialogs/dialogadcconfig.ui \
    Dialogs/dialogcycleconfig.ui \
    Dialogs/dialogplcconfig.ui \
    Modbus/dialogaddmodbusvar.ui \
    Modbus/dialogconfigmodbuscanal.ui \
    Modbus/dialogeditmodbusvar.ui \
    Modbus/dialogmodbusmaster.ui \
    mainwindow.ui \
    Dialogs\dialogldelementproperties.ui \
    Dialogs\dialogvarconfig.ui \
    Dialogs\dialogaddvar.ui \
    Dialogs\dialogeditnotsystemvar.ui \
    Dialogs\dialogeditsystemvar.ui \
    Dialogs\dialogrenamegroup.ui \
    Dialogs\dialogconstvarproperties.ui \
    Dialogs\dialogaddeditcomment.ui \
    dialogsearch.ui \
    Dialogs/dialogprojectconfig.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES +=
