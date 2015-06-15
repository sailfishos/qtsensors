TARGET = qtsensors_vbox
QT += sensors core gui

PLUGIN_TYPE = sensors
load(qt_plugin)
DESTDIR=vbox

HEADERS += vboxcommon.h\
           vboxaccelerometer.h\
           vboxlightsensor.h\
           vboxproximity.h\
           vboxorientationsensor.h\
           vboxrotationsensor.h

SOURCES += vboxcommon.cpp\
           vboxaccelerometer.cpp\
           vboxlightsensor.cpp\
           vboxproximity.cpp\
           vboxorientationsensor.cpp\
           vboxrotationsensor.cpp\
           main.cpp

OTHER_FILES = plugin.json

# SSU tool for Nemo Mobile, see https://github.com/nemomobile/ssu
contains(CONFIG,ssu): {
         LIBS += -lssu
         DEFINES += QT_USE_SSU
}

unix:!mac:!qnx:!android:LIBS+=-lrt

CONFIGFILES.files = Sensors.conf
CONFIGFILES.path = /etc/xdg/QtProject/
INSTALLS += CONFIGFILES

