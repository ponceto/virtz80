/*
 * emulator.cc - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "globals.h"
#include "emulator.h"

// ---------------------------------------------------------------------------
// some clock/time type aliases
// ---------------------------------------------------------------------------

using clock_type      = std::chrono::steady_clock;
using duration_type   = clock_type::duration;
using time_point_type = clock_type::time_point;

// ---------------------------------------------------------------------------
// core::Emulator
// ---------------------------------------------------------------------------

namespace core {

Emulator::Emulator()
    : base::Application("virtz80")
    , _vm(*this)
    , _ptime(clock_type::now())
    , _ctime(clock_type::now())
    , _dtime(0.0f)
{
    _vm.reset();
}

auto Emulator::main() -> void
{
    auto loop = [&]() -> void
    {
        _ptime = _ctime;
        _ctime = clock_type::now();
        _dtime = std::chrono::duration<float>(_ctime - _ptime).count();
        _vm.clock();
    };

#ifdef __EMSCRIPTEN__
    if(_quit == false) {
        loop();
    }
#else
    while(_quit == false) {
        loop();
    }
#endif
}

auto Emulator::quit() -> void
{
    if(_quit == false) {
        _quit = true;
    }
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
