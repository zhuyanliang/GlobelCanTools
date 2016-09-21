#-------------------------------------------------
#
# Project created by QtCreator 2016-07-04T15:31:38
#
#-------------------------------------------------

QT       += core gui sql

TARGET = GlobelCanTools
TEMPLATE = app

CONFIG   += qaxcontainer

SOURCES +=\
    VoltageUI.cpp \
    CellMinMaxInfo.cpp \
    AmpereMeter.cpp \
    BatterySoc.cpp \
    IndictorLight.cpp \
    DataProcess.cpp \
    CanBus.cpp \
    DevSetDialog.cpp \
    ExcelEngine.cpp \
    TableView2Excel.cpp \
    ParseHex.cpp \
    DownLoadUi.cpp \
    Main.cpp \
    SysPraModel.cpp \
    XmlConfig.cpp \
    MainWindowUI.cpp \
    MainWindowSlots.cpp

HEADERS  += MainWindow.h \
    VoltageUI.h \
    CellMinMaxInfo.h \
    AmpereMeter.h \
    BatterySoc.h \
    IndictorLight.h \
    DataProcess.h \
    ControlCAN.h \
    DataType.h \
    CanBus.h \
    DebugConfig.h \
    DevSetDialog.h \
    ExcelEngine.h \
    TableView2Excel.h \
    DownLoadUi.h \
    ParseHex.h \
    SysPraModel.h \
    Header.h \
    XmlConfig.h

FORMS    += mainwindow.ui \
    devsetdialog.ui \
    DownLoadUi.ui \
    SysPraModel.ui

RESOURCES += \
    src.qrc



unix:!macx:!symbian|win32: LIBS += -L$$PWD/ -lControlCAN

INCLUDEPATH += $$PWD/
DEPENDPATH += $$PWD/
