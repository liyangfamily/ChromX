include(ChromX.pri)

#version check qt
!minQtVersion(5, 12, 0) {
    message("Cannot build $$IDE_DISPLAY_NAME with Qt version $${QT_VERSION}.")
    error("Use at least Qt 5.12.0.")
}

TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += src \
    3rdparty/ChromXControlEngine
