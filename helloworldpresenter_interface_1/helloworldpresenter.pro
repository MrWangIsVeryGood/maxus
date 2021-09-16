#
# Helloworld presenter
#
# This example shows you how to implement a minimum presenter using the SDK and
# shows you how to share code between QML world and C++ side
#

include(../presenter_common.pri)
include(../../opencv.pri)

QT +=  quickwidgets

HEADERS += \
    vxhelloworldpresenter.h
SOURCES += \
    vxhelloworldpresenter.cpp

TARGET = helloworldpresenter

VERSION = 2.0.42

RESOURCES += \
    helloworldqml.qrc

DISTFILES +=
