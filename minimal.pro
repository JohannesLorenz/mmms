######################################################################
# Automatically generated by qmake (2.01a) Sa. Mai 10 07:29:55 2014
######################################################################

TEMPLATE = subdirs

# Directories

INCLUDEPATH += src/include/ \
	submodules/


# Input

HEADERS += src/include/plugin.h \
	src/include/project.h \
	src/include/instrument.h \
	src/include/utils.h \
	src/include/tuple_helpers.h \
	src/include/daw.h \
	src/include/util.h \
	src/include/lfo.h \
	src/include/types.h \
	src/include/osc_string.h \
	src/include/command.h \
	src/include/effect.h \
	src/include/work_queue.h \
        src/include/event_roll.h \
	src/include/recorder.h \
	src/include/soundfile.h \
	src/include/jack.h \
	src/include/audio.h \
	src/include/audio_instrument.h \
	src/include/io.h \
	src/include/jack_player.h \
	src/include/sample.h \
	src/include/bars.h \
	src/core/engine.h \
	src/core/jack_engine.h \
	src/include/fraction.h \
	src/include/debug.h \
	src/include/port_fwd.h \
	src/include/command_tools.h \
	src/core/player.h \
	src/include/atomic.h \
	src/include/spinlock.h \
	src/include/mports.h \
	src/include/audio_sink.h \
	src/include/scales.h \
	src/include/node.h \
	src/include/protocol.h \
	src/include/notes.h \
        src/core/os_engine.h \
        src/include/simple.h

SOURCES += src/core/main.cpp \
	src/include/plugin.cpp \
	src/include/project.cpp \
	src/include/instrument.cpp \
        src/include/daw.cpp \
        src/include/lfo.cpp \
	src/test/test_str.cpp \
	src/include/osc_string.cpp \
        src/include/event_roll.cpp \
	src/include/command.cpp \
	src/include/recorder.cpp \
	src/include/soundfile.cpp \
	src/include/jack.cpp \
	src/include/audio_instrument.cpp \
        src/include/audio.cpp \
	src/include/io.cpp \
	src/include/effect.cpp \
	src/include/jack_player.cpp \
	src/test/commands.cpp \
	src/test/maths.cpp \
	src/core/engine.cpp \
	src/core/jack_engine.cpp \
	src/include/fraction.cpp \
	src/songs/debug_proj.cpp \
        src/include/command_tools.cpp \
	src/core/player.cpp \
	src/include/sample.cpp \
	src/include/node.cpp \
	src/include/protocol.cpp \
	src/core/os_engine.cpp \
        src/include/bars.cpp \
        src/include/ports.cpp

OTHER_FILES += INSTALL.md \
	CMakeLists.txt \
	src/CMakeLists.txt \
	src/core/CMakeLists.txt \
	src/songs/CMakeLists.txt \
	src/include/CMakeLists.txt \
	src/test/CMakeLists.txt \
	src/config.h.in \
	README.md \
	RULES.md \
	src/minimal.pc.in


