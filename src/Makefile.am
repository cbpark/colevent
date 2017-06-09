AM_CXXFLAGS = -std=c++11
if DEBUG
AM_CXXFLAGS += -DDEBUG -O0 -Wall -Wextra -pedantic
endif
if USE_ROOT
AM_CXXFLAGS += $(ROOTCFLAGS)
endif

lib_LTLIBRARIES        = libcolevent.la
libcolevent_la_SOURCES = \
	kinematics.cc \
	lhef/event.cc lhef/lhef.cc lhef/parser.cc lhef/particle.cc
if USE_ROOT
libcolevent_la_LIBADD  = -L$(ROOTLIBDIR) $(ROOTLIBS)
endif

nobase_pkginclude_HEADERS = \
	constants.h kinematics.h \
	lhef/event.h lhef/lhef.h lhef/parser.h lhef/particle.h lhef/pid.h

if DEBUG
noinst_bindir = $(abs_top_builddir)/src
noinst_bin_PROGRAMS = test_parse_lhef test_render_lhef

test_parse_lhef_SOURCES = test_parse_lhef.cc
test_parse_lhef_LDADD   = libcolevent.la

test_render_lhef_SOURCES = test_render_lhef.cc
test_render_lhef_LDADD   = libcolevent.la

if USE_ROOT
test_parse_lhef_LDADD += -L$(ROOTLIBDIR) $(ROOTLIBS)
test_render_lhef_LDADD += -L$(ROOTLIBDIR) $(ROOTLIBS)
endif
endif
