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
    templatedecoderfactory.cpp \
    guardconverter_tests.cpp \
    guardconverter.cpp \
    lineeditwithhintlist.cpp \
    file.cpp \
    file_link.cpp \
    file_parser.cpp \
    form2.cpp

HEADERS += \
    form.h \
    templateloader.h \
    templateloader_tests.h \
    templatedecoder.h \
    templatedecoder_tests.h \
    templatedecoderfactory.h \
    guardconverter_tests.h \
    guardconverter.h \
    lineeditwithhintlist.h \
    file.h \
    file_link.h \
    file_data.h \
    file_parser.h \
    form2.h

RESOURCES += \
    FileTempaltes/filetemplates.qrc \
    FileTempaltesTests/filetemplatestests.qrc
