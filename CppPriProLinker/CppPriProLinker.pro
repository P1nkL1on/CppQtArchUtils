QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CppPriProLinker
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
    file_cpp.cpp \
    file_cpp_factory.cpp \
    file_pro.cpp \
    file_pro_factory.cpp \
    guard_converter.cpp \
    guard_converter_tests.cpp \
    lineedit_withhintlist.cpp \
    main.cpp \
    form.cpp \
    ref_class.cpp \
    ref_file.cpp \
    template_decoder.cpp \
    template_decoder_tests.cpp \
    template_loader.cpp \
    file.cpp \
    form2.cpp \
    template_loader_tests.cpp \
    tokenizer.cpp \
    token_parser.cpp \
    tokenizer_tests.cpp

HEADERS += \
    file_cpp.h \
    file_cpp_factory.h \
    file_factory.h \
    file_pro.h \
    file_pro_factory.h \
    form.h \
    guard_converter.h \
    guard_converter_tests.h \
    lineedit_withhintlist.h \
    ref_class.h \
    ref_file.h \
    template_decoder.h \
    template_decoder_tests.h \
    template_loader.h \
    file.h \
    file_data.h \
    form2.h \
    template_loader_tests.h \
    token.h \
    tokenizer.h \
    token_parser.h \
    tokenizer_tests.h

RESOURCES += \
    FileTempaltes/filetemplates.qrc \
    FileTempaltesTests/filetemplatestests.qrc
