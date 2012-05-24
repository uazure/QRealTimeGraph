CONFIG += designer \
    plugin \
    debug_and_release
TARGET = $$qtLibraryTarget(qrealtimegraphplugin)
TEMPLATE = lib
HEADERS = qrealtimegraphplugin.h
SOURCES = qrealtimegraphplugin.cpp
RESOURCES = icons.qrc
LIBS += -L.
target.path = $$[QT_INSTALL_PLUGINS]/designer
INSTALLS += target
include(qrealtimegraph.pri)
OTHER_FILES += README
