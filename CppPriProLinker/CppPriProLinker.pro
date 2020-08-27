QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CppPriProLinker
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
    guard_converter.cpp \
    guard_converter_tests.cpp \
    lineedit_withhintlist.cpp \
    main.cpp \
    form.cpp \
    template_decoder.cpp \
    template_decoder_tests.cpp \
    template_loader.cpp \
    file.cpp \
    file_link.cpp \
    file_parser.cpp \
    form2.cpp \
    file_tokenizer.cpp \
    template_loader_tests.cpp

HEADERS += \
    form.h \
    guard_converter.h \
    guard_converter_tests.h \
    lineedit_withhintlist.h \
    template_decoder.h \
    template_decoder_tests.h \
    template_loader.h \
    file.h \
    file_link.h \
    file_data.h \
    file_parser.h \
    form2.h \
    file_tokenizer.h \
    template_loader_tests.h

RESOURCES += \
    FileTempaltes/filetemplates.qrc \
    FileTempaltesTests/filetemplatestests.qrc
