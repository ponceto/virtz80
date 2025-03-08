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
CPPFLAGS = -I. -I$(TOPDIR)/src -D_DEFAULT_SOURCE -D_FORTIFY_SOURCE=2
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

build: build_zexall build_zexdoc
	@echo "=== $@ ok ==="

clean: clean_zexall clean_zexdoc
	@echo "=== $@ ok ==="

# ----------------------------------------------------------------------------
# zexall files
# ----------------------------------------------------------------------------

zexall_PROGRAM = zexall.bin

zexall_SOURCES = \
	zexall.cc \
	$(NULL)

zexall_OBJECTS = \
	zexall.o \
	$(NULL)

zexall_LDADD = \
	$(NULL)

zexall_CLEANFILES = \
	zexall.rom \
	$(NULL)

# ----------------------------------------------------------------------------
# build zexall
# ----------------------------------------------------------------------------

build_zexall: $(zexall_PROGRAM)

$(zexall_PROGRAM): $(zexall_OBJECTS)
	$(LD) $(LDFLAGS) -o $(zexall_PROGRAM) $(zexall_OBJECTS) $(zexall_LDADD)

# ----------------------------------------------------------------------------
# clean zexall
# ----------------------------------------------------------------------------

clean_zexall:
	$(RM) $(RMFLAGS) $(zexall_OBJECTS) $(zexall_PROGRAM) $(zexall_CLEANFILES)

# ----------------------------------------------------------------------------
# zexdoc files
# ----------------------------------------------------------------------------

zexdoc_PROGRAM = zexdoc.bin

zexdoc_SOURCES = \
	zexdoc.cc \
	$(NULL)

zexdoc_OBJECTS = \
	zexdoc.o \
	$(NULL)

zexdoc_LDADD = \
	$(NULL)

zexdoc_CLEANFILES = \
	zexdoc.rom \
	$(NULL)

# ----------------------------------------------------------------------------
# build zexdoc
# ----------------------------------------------------------------------------

build_zexdoc: $(zexdoc_PROGRAM)

$(zexdoc_PROGRAM): $(zexdoc_OBJECTS)
	$(LD) $(LDFLAGS) -o $(zexdoc_PROGRAM) $(zexdoc_OBJECTS) $(zexdoc_LDADD)

# ----------------------------------------------------------------------------
# clean zexdoc
# ----------------------------------------------------------------------------

clean_zexdoc:
	$(RM) $(RMFLAGS) $(zexdoc_OBJECTS) $(zexdoc_PROGRAM) $(zexdoc_CLEANFILES)

# ----------------------------------------------------------------------------
# End-Of-File
# ----------------------------------------------------------------------------
