######################################################################
# Automatically generated by qmake (2.01a) Sa. Mai 10 07:29:55 2014
######################################################################

TEMPLATE = subdirs

# Directories

INCLUDEPATH += src/include/ \
	rtosc/include

# Input

HEADERS += src/core/plugin.h \
	src/include/project.h \
	src/include/instrument.h \
	src/include/ports.h \
	src/core/loaded_project.h

SOURCES += src/core/main.cpp \
	src/core/plugin.cpp \
	src/include/project.cpp \
	src/songs/demo.cpp \
	src/include/instrument.cpp \
	src/include/ports.cpp \
	src/core/loaded_project.cpp

OTHER_FILES += CMakeLists.txt \
	src/CMakeLists.txt \
	src/core/CMakeLists.txt \
	src/songs/CMakeLists.txt \
	src/include/CMakeLists.txt

