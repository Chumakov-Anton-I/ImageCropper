TEMPLATE = app

TARGET = imgcrop

CONFIG += qt c++11

QT += core gui widgets

SOURCES += \
    cframe.cpp \
    main.cpp \
    mainwindow.cpp \
    tcanvas.cpp

HEADERS += \
    cframe.h \
    mainwindow.h \
    tcanvas.h

msvc:{
    QMAKE_CXXFLAGS_RELEASE += /O2
}
gcc: {
    QMAKE_CXXFLAGS_RELEASE += -O2
}
clang:{
    QMAKE_CXXFLAGS_RELEASE += -O3
}

RESOURCES += \
    resources.qrc
