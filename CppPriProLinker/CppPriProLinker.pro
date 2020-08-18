QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CppPriProLinker
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
    main.cpp \
    form.cpp \
    templateloader.cpp \
    templateloader_tests.cpp \
    templatedecoder.cpp \
    templatedecoder_tests.cpp \
    templatedecoderfactory.cpp

HEADERS += \
    form.h \
    templateloader.h \
    filedata.h \
    templateloader_tests.h \
    templatedecoder.h \
    templatedecoder_tests.h \
    templatedecoderfactory.h

RESOURCES += \
    FileTempaltes/filetemplates.qrc \
    FileTempaltesTests/filetemplatestests.qrc
