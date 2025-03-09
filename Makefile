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
	src/main.cc \
	src/app/application.cc \
	src/app/console.cc \
	src/app/globals.cc \
	src/app/program.cc \
	src/app/terminal.cc \
	src/app/emulator.cc \
	src/dev/cpu/cpu-core.cc \
	src/dev/mmu/mmu-core.cc \
	src/dev/vdu/vdu-core.cc \
	src/dev/sio/sio-core.cc \
	src/emu/virtual-machine.cc \
	$(NULL)

virtz80_HEADERS = \
	src/main.h \
	src/app/application.h \
	src/app/console.h \
	src/app/globals.h \
	src/app/program.h \
	src/app/terminal.h \
	src/app/emulator.h \
	src/dev/cpu/cpu-core.h \
	src/dev/mmu/mmu-core.h \
	src/dev/vdu/vdu-core.h \
	src/dev/sio/sio-core.h \
	src/emu/virtual-machine.h \
	$(NULL)

virtz80_OBJECTS = \
	src/main.o \
	src/app/application.o \
	src/app/console.o \
	src/app/globals.o \
	src/app/program.o \
	src/app/terminal.o \
	src/app/emulator.o \
	src/dev/cpu/cpu-core.o \
	src/dev/mmu/mmu-core.o \
	src/dev/vdu/vdu-core.o \
	src/dev/sio/sio-core.o \
	src/emu/virtual-machine.o \
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
