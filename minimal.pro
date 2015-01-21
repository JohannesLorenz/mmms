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
	src/core/loaded_project.h \
	src/include/utils.h \
	src/include/tuple_helpers.h \
	src/include/daw.h \
	src/include/util.h \
	src/include/lfo.h \
	src/include/types.h \
	src/include/zynaddsubfx.h \
    src/include/osc_string.h \
    src/include/lo_port.h \
      src/include/command.h \
    src/include/effect.h \
src/include/work_queue.h \
src/include/daw_visit.h \
src/include/note_line.h \
    src/include/ports.h

SOURCES += src/core/main.cpp \
	src/core/plugin.cpp \
	src/include/project.cpp \
	src/songs/demo.cpp \
	src/include/instrument.cpp \
	src/core/loaded_project.cpp \
	src/core/lfo.cpp \
	src/test/test_str.cpp \
	src/include/zynaddsubfx.cpp \
        src/include/osc_string.cpp \
    src/include/lo_port.cpp \
    src/include/daw_visit.cpp \
    src/include/note_line.cpp

OTHER_FILES += INSTALL.txt \
	README.txt \
	CMakeLists.txt \
	src/CMakeLists.txt \
	src/core/CMakeLists.txt \
	src/songs/CMakeLists.txt \
	src/include/CMakeLists.txt \
	src/test/CMakeLists.txt \
	src/config.h.in


