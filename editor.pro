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

# QXlsx code for Application Qt project
QXLSX_PARENTPATH=./QXlsx/         # current QXlsx path is . (. means curret directory)
QXLSX_HEADERPATH=./QXlsx/header/  # current QXlsx header path is ./header/
QXLSX_SOURCEPATH=./QXlsx/source/  # current QXlsx source path is ./source/
include(./QXlsx/QXlsx.pri)

SOURCES += \
    Dialogs/dialogadcconfig.cpp \
    Dialogs/dialogcycleconfig.cpp \
    Dialogs/dialogdiconfig.cpp \
    Dialogs/dialogdoconfig.cpp \
    Dialogs/dialogplcconfig.cpp \
    Dialogs/dialogsearchvar.cpp \
    Dialogs/dialogsensorconfig.cpp \
    Elements/counterelement.cpp \
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
    Modbus/modbusrequestlist.cpp \
    Modbus/modbusvar.cpp \
    Modbus/modbusvarsstorage.cpp \
    QXlsx/source/xlsxabstractooxmlfile.cpp \
    QXlsx/source/xlsxabstractsheet.cpp \
    QXlsx/source/xlsxcell.cpp \
    QXlsx/source/xlsxcellformula.cpp \
    QXlsx/source/xlsxcelllocation.cpp \
    QXlsx/source/xlsxcellrange.cpp \
    QXlsx/source/xlsxcellreference.cpp \
    QXlsx/source/xlsxchart.cpp \
    QXlsx/source/xlsxchartsheet.cpp \
    QXlsx/source/xlsxcolor.cpp \
    QXlsx/source/xlsxconditionalformatting.cpp \
    QXlsx/source/xlsxcontenttypes.cpp \
    QXlsx/source/xlsxdatavalidation.cpp \
    QXlsx/source/xlsxdatetype.cpp \
    QXlsx/source/xlsxdocpropsapp.cpp \
    QXlsx/source/xlsxdocpropscore.cpp \
    QXlsx/source/xlsxdocument.cpp \
    QXlsx/source/xlsxdrawing.cpp \
    QXlsx/source/xlsxdrawinganchor.cpp \
    QXlsx/source/xlsxformat.cpp \
    QXlsx/source/xlsxmediafile.cpp \
    QXlsx/source/xlsxnumformatparser.cpp \
    QXlsx/source/xlsxrelationships.cpp \
    QXlsx/source/xlsxrichstring.cpp \
    QXlsx/source/xlsxsharedstrings.cpp \
    QXlsx/source/xlsxsimpleooxmlfile.cpp \
    QXlsx/source/xlsxstyles.cpp \
    QXlsx/source/xlsxtheme.cpp \
    QXlsx/source/xlsxutility.cpp \
    QXlsx/source/xlsxworkbook.cpp \
    QXlsx/source/xlsxworksheet.cpp \
    QXlsx/source/xlsxzipreader.cpp \
    QXlsx/source/xlsxzipwriter.cpp \
    SourceBuilder/adcmanager.cpp \
    ainsensor.cpp \
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
    elementlibrary.cpp \
    transcripter.cpp

HEADERS += \
    Dialogs/dialogadcconfig.h \
    Dialogs/dialogcycleconfig.h \
    Dialogs/dialogdiconfig.h \
    Dialogs/dialogdoconfig.h \
    Dialogs/dialogplcconfig.h \
    Dialogs/dialogsearchvar.h \
    Dialogs/dialogsensorconfig.h \
    Elements/counterelement.h \
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
    Modbus/modbusrequestlist.h \
    Modbus/modbusvar.h \
    Modbus/modbusvarsstorage.h \
    QXlsx/header/xlsxabstractooxmlfile.h \
    QXlsx/header/xlsxabstractooxmlfile_p.h \
    QXlsx/header/xlsxabstractsheet.h \
    QXlsx/header/xlsxabstractsheet_p.h \
    QXlsx/header/xlsxcell.h \
    QXlsx/header/xlsxcell_p.h \
    QXlsx/header/xlsxcellformula.h \
    QXlsx/header/xlsxcellformula_p.h \
    QXlsx/header/xlsxcelllocation.h \
    QXlsx/header/xlsxcellrange.h \
    QXlsx/header/xlsxcellreference.h \
    QXlsx/header/xlsxchart.h \
    QXlsx/header/xlsxchart_p.h \
    QXlsx/header/xlsxchartsheet.h \
    QXlsx/header/xlsxchartsheet_p.h \
    QXlsx/header/xlsxcolor_p.h \
    QXlsx/header/xlsxconditionalformatting.h \
    QXlsx/header/xlsxconditionalformatting_p.h \
    QXlsx/header/xlsxcontenttypes_p.h \
    QXlsx/header/xlsxdatavalidation.h \
    QXlsx/header/xlsxdatavalidation_p.h \
    QXlsx/header/xlsxdatetype.h \
    QXlsx/header/xlsxdocpropsapp_p.h \
    QXlsx/header/xlsxdocpropscore_p.h \
    QXlsx/header/xlsxdocument.h \
    QXlsx/header/xlsxdocument_p.h \
    QXlsx/header/xlsxdrawing_p.h \
    QXlsx/header/xlsxdrawinganchor_p.h \
    QXlsx/header/xlsxformat.h \
    QXlsx/header/xlsxformat_p.h \
    QXlsx/header/xlsxglobal.h \
    QXlsx/header/xlsxmediafile_p.h \
    QXlsx/header/xlsxnumformatparser_p.h \
    QXlsx/header/xlsxrelationships_p.h \
    QXlsx/header/xlsxrichstring.h \
    QXlsx/header/xlsxrichstring_p.h \
    QXlsx/header/xlsxsharedstrings_p.h \
    QXlsx/header/xlsxsimpleooxmlfile_p.h \
    QXlsx/header/xlsxstyles_p.h \
    QXlsx/header/xlsxtheme_p.h \
    QXlsx/header/xlsxutility_p.h \
    QXlsx/header/xlsxworkbook.h \
    QXlsx/header/xlsxworkbook_p.h \
    QXlsx/header/xlsxworksheet.h \
    QXlsx/header/xlsxworksheet_p.h \
    QXlsx/header/xlsxzipreader_p.h \
    QXlsx/header/xlsxzipwriter_p.h \
    SourceBuilder/adcmanager.h \
    ainsensor.h \
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
    elementlibrary.h \
    transcripter.h

FORMS += \
    Dialogs/dialogadcconfig.ui \
    Dialogs/dialogcycleconfig.ui \
    Dialogs/dialogdiconfig.ui \
    Dialogs/dialogdoconfig.ui \
    Dialogs/dialogplcconfig.ui \
    Dialogs/dialogsearchvar.ui \
    Dialogs/dialogsensorconfig.ui \
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

DISTFILES += \
    QXlsx/QXlsx.pri

SUBDIRS += \
    QXlsx/QXlsx.pro
