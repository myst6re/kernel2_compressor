QT += core gui widgets

TARGET = kernel2_compressor
TEMPLATE = app

DEFINES += QT_NO_DEBUG_OUTPUT

SOURCES += main.cpp \
    mainwindow.cpp \
    core/compressor.cpp \
    core/kernel2.cpp \
    core/ff7text.cpp \
    core/lzs.cpp

HEADERS += mainwindow.h \
    core/compressor.h \
    core/kernel2.h \
    core/ff7text.h \
    core/lzs.h \
    global.h

OTHER_FILES += kernel2_compressor.rc \
    deploy.bat \
    .clang-format \
    .travis.yml \
    appveyor.yml

DISTFILES += COPYING \
    README.md

win32 {
    RC_FILE = kernel2_compressor.rc
}

# Console version
configCONSOLE {
    CONFIG += console
    CONFIG -= app_bundle
    QT -= winextras widgets gui

    DEFINES += KERNEL2_CONSOLE

    SOURCES -= mainwindow.cpp

    HEADERS -= mainwindow.h

    SOURCES += maincli.cpp

    HEADERS += maincli.h

}

# Tests
configTEST {
    QT += testlib
    CONFIG += testcase

    SOURCES += tests/test.cpp \
        tests/kernel2generator.cpp \
        tests/kernel2test.cpp

    HEADERS += tests/kernel2generator.h \
        tests/kernel2test.h

    SOURCES -= main.cpp
}
