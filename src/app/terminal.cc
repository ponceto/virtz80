/*
 * terminal.cc - Copyright (c) 2001-2025 - Olivier Poncet
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <cstdarg>
#include <chrono>
#include <thread>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <unistd.h>
#include <termios.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "terminal.h"

// ---------------------------------------------------------------------------
// Terminal
// ---------------------------------------------------------------------------

namespace app {

Terminal::Terminal(int fd0, int fd1, int fd2)
    : _filedes{fd0, fd1, fd2}
    , _termios()
{
    get_attributes(_filedes[0], _termios[0]);
    get_attributes(_filedes[1], _termios[1]);
    get_attributes(_filedes[2], _termios[2]);
}

Terminal::~Terminal()
{
    set_attributes(_filedes[0], _termios[0]);
    set_attributes(_filedes[1], _termios[1]);
    set_attributes(_filedes[2], _termios[2]);
}

auto Terminal::get_attributes(int fd, termios_type& attributes) -> void
{
    if((fd >= 0) && (::isatty(fd) != 0)) {
        const int rc = ::tcgetattr(fd, &attributes);
        if(rc != 0) {
            throw std::runtime_error("tcgetattr() has failed");
        }
    }
}

auto Terminal::set_attributes(int fd, termios_type& attributes) -> void
{
    if((fd >= 0) && (::isatty(fd) != 0)) {
        const int rc = ::tcsetattr(fd, TCSANOW, &attributes);
        if(rc != 0) {
            throw std::runtime_error("tcsetattr() has failed");
        }
    }
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
