/*
 * main.cc - Copyright (c) 2001-2025 - Olivier Poncet
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
#include "app/console.h"
#include "app/program.h"
#include "app/terminal.h"

using namespace app;

// ---------------------------------------------------------------------------
// <anonymous>::run
// ---------------------------------------------------------------------------

namespace {

auto run(const ArgList& args) -> void
{
    const Terminal terminal(0, 1, 2);

    if(Program::init(args) != false) {
        Program::main(args);
    }
    else {
        Program::help(args);
    }
}

}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    const ArgList args(argv, argv + argc);

    try {
        run(args);
    }
    catch(const std::exception& e) {
        Console::error(e.what());
        return EXIT_FAILURE;
    }
    catch(...) {
        Console::error("error!");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
