# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

TARGET = FordFulkersonMaximumFlow

CONFIG -= qt

HEADERS = \
   $$PWD/include/definitions.hpp \
   $$PWD/include/edge.hpp \
   $$PWD/include/Graph.hpp \
   $$PWD/include/hollowheap.h \
   $$PWD/include/HollowHeap.hpp \
   $$PWD/include/memory_used.hpp \
   $$PWD/include/nheap.hpp \
   $$PWD/include/read_dimacs.hpp \
   $$PWD/include/ScopeGuard.hpp \
   $$PWD/include/utils.hpp

SOURCES = \
   $$PWD/src/Graph.cpp \
   $$PWD/src/hollowheap.cc \
   $$PWD/src/main.cpp \
   $$PWD/src/memory_used.cpp

INCLUDEPATH = \
    $$PWD/include

#DEFINES = 

