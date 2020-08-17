QT       += core gui testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CppPriProLinker
TEMPLATE = app

SOURCES += \
    main.cpp \
    form.cpp \
    templateloader.cpp \
    templateloader_tests.cpp

HEADERS += \
    form.h \
    templateloader.h \
    filedata.h \
    templateloader_tests.h

RESOURCES += \
    FileTempaltes/filetemplates.qrc \
    FileTempaltesTests/filetemplatestests.qrc
