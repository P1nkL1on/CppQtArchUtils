QT += concurrent core
CONFIG += c++17
CONFIG -= console
CONFIG -= app_bundle

CONFIG += object_parallel_to_source

QMAKE_CXXFLAGS += -std=c++17
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS += -fopenmp

SOURCES += \
        main.cpp
