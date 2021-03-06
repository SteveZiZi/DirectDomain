# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

rootdir=../..
PROJECT=SmartBuilding

INCLUDEPATH += $${GeneratedFileDir} \
               $${rootdir} \
               . 

DESTDIR = $${rootdir}/bin
LIBS += -L$${rootdir}/lib \
        -L$${rootdir}/bin
        

TEMPLATE = app
QT += core gui sql xml
CONFIG += debug_and_release
DEFINES += QT_DLL QT_XML_LIB QT_SQL_LIB QT_CORE_LIB QT_GUI_LIB

GeneratedFileDir=$${rootdir}/build/intermediate/$${PROJECT}
DEPENDPATH += .
UI_DIR  += $${GeneratedFileDir}
RCC_DIR += $${GeneratedFileDir}


CONFIG(debug, debug|release) {
    TARGET = $${PROJECT}_d
    MOC_DIR += $${GeneratedFileDir}/debug
    INCLUDEPATH += $${GeneratedFileDir}/Debug
    OBJECTS_DIR += $${GeneratedFileDir}/Debug
    LIBS += -lvKeyBoard_d
} else {
    DEFINES += QT_NO_DEBUGs
    TARGET = $${PROJECT}
    INCLUDEPATH += $${GeneratedFileDir}/Release
    MOC_DIR += $${GeneratedFileDir}/Release
    OBJECTS_DIR += $${GeneratedFileDir}/Release
    LIBS += -lvKeyBoard
}

include(SmartBuilding.pri)
