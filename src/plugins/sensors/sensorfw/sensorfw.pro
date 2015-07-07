TARGET = qtsensors_sensorfw
QT = core sensors network dbus

PLUGIN_TYPE = sensors
load(qt_plugin)

include(sensorfw.pri)


CONFIG += link_pkgconfig
PKGCONFIG += sensord-qt5

OTHER_FILES = plugin.json
