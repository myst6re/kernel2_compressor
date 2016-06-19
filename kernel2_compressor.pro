QT += core gui widgets

TARGET = kernel2_compressor
TEMPLATE = app


SOURCES += main.cpp \
    mainwindow.cpp \
    core/compressor.cpp \
    core/kernel2.cpp \
    core/ff7text.cpp \
    core/lzs.cpp

HEADERS  += mainwindow.h \
    core/compressor.h \
    core/kernel2.h \
    core/ff7text.h \
    core/lzs.h \
    global.h

OTHER_FILES += kernel2_compressor.rc \
    deploy.bat \
    COPYING

DISTFILES += COPYING \
    README.md

RESOURCES += kernel2_compressor.qrc \
    kernel2_compressor.qrc

win32 {
    RC_FILE = kernel2_compressor.rc
}
