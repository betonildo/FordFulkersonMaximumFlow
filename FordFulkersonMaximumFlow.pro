# Created by and for Qt Creator. This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

TARGET = FordFulkersonMaximumFlow

# Remove qt default libraries for a c++ default project
CONFIG -= qt

HEADERS = \
   $$PWD/include/definitions.hpp \
   $$PWD/include/Element.hpp \
   $$PWD/include/Graph.hpp \
   $$PWD/include/HollowHeap.hpp \
   $$PWD/include/memory_used.hpp \
   $$PWD/include/Node.hpp \
   $$PWD/include/read_dimacs.hpp \
   $$PWD/include/ScopeGuard.hpp \
   $$PWD/include/utils.hpp \
    include/hollowheap.h \
    include/nheap.hpp \
    include/edge.hpp

SOURCES = \
   $$PWD/src/Graph.cpp \
   $$PWD/src/main.cpp \
   $$PWD/src/memory_used.cpp \
    src/hollowheap.cc

INCLUDEPATH = \
    $$PWD/include

#DEFINES = 

