include($$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri))
include(../../ChromX.pri)
include(ChromXApp.pri)

QT += core gui network serialport opengl openglextensions printsupport
QT += concurrent
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
#win32:RC_ICONS = Resources/Normal/ChromXApp.ico
#osx:ICON = Resources/Normal/ChromXApp.icns

TARGET = $$PRO_TARGET
DESTDIR = $$PRO_BIN_PATH

INCLUDEPATH += \

exists (../../.git) {
    GIT_BRANCH   = $$system(git rev-parse --abbrev-ref HEAD)
    GIT_SHA      = $$system(git rev-parse --short=8 HEAD)
    GIT_TIME     = $$system(git log --pretty=format:\"%cd\" --date=format:\"%Y%m%d-%H%M%S\" -1 HEAD)

    GIT_BUILD_INFO = "$${GIT_SHA}-$${GIT_TIME}"
} else {
    GIT_BRANCH         = --
    GIT_SHA            = --
    GIT_TIME           = --
    GIT_BUILD_INFO     = --
}
message(git time = $$GIT_TIME)
message(git sha = $$GIT_SHA)

DEFINES += GIT_BUILD_SHA=\"\\\"$$GIT_SHA\\\"\"
DEFINES += GIT_BUILD_TIME=\"\\\"$$GIT_TIME\\\"\"


