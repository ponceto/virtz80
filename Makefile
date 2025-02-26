#
# Makefile - Copyright (c) 2001-2025 - Olivier Poncet
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# ----------------------------------------------------------------------------
# global environment
# ----------------------------------------------------------------------------

TOPDIR   = $(CURDIR)
OPTLEVEL = -O2 -g
WARNINGS = -Wall
EXTRAS   = -pthread
CC       = gcc
CFLAGS   = -std=c99 $(OPTLEVEL) $(WARNINGS) $(EXTRAS)
CXX      = g++
CXXFLAGS = -std=c++14 $(OPTLEVEL) $(WARNINGS) $(EXTRAS)
CPP      = cpp
CPPFLAGS = -I. -I$(TOPDIR)/src -D_DEFAULT_SOURCE -D_FORTIFY_SOURCE=2 -DLSB_FIRST
LD       = g++
LDFLAGS  = -L.
CP       = cp
CPFLAGS  = -f
RM       = rm
RMFLAGS  = -f

# ----------------------------------------------------------------------------
# default rules
# ----------------------------------------------------------------------------

.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

# ----------------------------------------------------------------------------
# global targets
# ----------------------------------------------------------------------------

all: build

build: build_virtz80
	@echo "=== $@ ok ==="

clean: clean_virtz80
	@echo "=== $@ ok ==="

# ----------------------------------------------------------------------------
# virtz80 files
# ----------------------------------------------------------------------------

virtz80_PROGRAM = virtz80.bin

virtz80_SOURCES = \
	src/base/application.cc \
	src/base/console.cc \
	src/globals.cc \
	src/program.cc \
	src/emulator.cc \
	src/emu/machine.cc \
	src/emu/cpu/cpu-core.cc \
	src/emu/mmu/mmu-core.cc \
	src/emu/pmu/pmu-core.cc \
	src/emu/vdu/vdu-core.cc \
	$(NULL)

virtz80_HEADERS = \
	src/base/application.h \
	src/base/console.h \
	src/globals.h \
	src/program.h \
	src/emulator.h \
	src/emu/machine.h \
	src/emu/cpu/cpu-core.h \
	src/emu/mmu/mmu-core.h \
	src/emu/pmu/pmu-core.h \
	src/emu/vdu/vdu-core.h \
	$(NULL)

virtz80_OBJECTS = \
	src/base/application.o \
	src/base/console.o \
	src/globals.o \
	src/program.o \
	src/emulator.o \
	src/emu/machine.o \
	src/emu/cpu/cpu-core.o \
	src/emu/mmu/mmu-core.o \
	src/emu/pmu/pmu-core.o \
	src/emu/vdu/vdu-core.o \
	$(NULL)

virtz80_LDFLAGS = \
	$(NULL)

virtz80_LDADD = \
	-lpthread \
	-lm \
	$(NULL)

virtz80_CLEANFILES = \
	virtz80.bin \
	virtz80.data \
	virtz80.html \
	virtz80.wasm \
	virtz80.js \
	$(NULL)

# ----------------------------------------------------------------------------
# build virtz80
# ----------------------------------------------------------------------------

build_virtz80: $(virtz80_PROGRAM)

$(virtz80_PROGRAM): $(virtz80_OBJECTS)
	$(LD) $(LDFLAGS) $(virtz80_LDFLAGS) -o $(virtz80_PROGRAM) $(virtz80_OBJECTS) $(virtz80_LDADD)

# ----------------------------------------------------------------------------
# clean virtz80
# ----------------------------------------------------------------------------

clean_virtz80:
	$(RM) $(RMFLAGS) $(virtz80_OBJECTS) $(virtz80_PROGRAM) $(virtz80_CLEANFILES)

# ----------------------------------------------------------------------------
# End-Of-File
# ----------------------------------------------------------------------------
