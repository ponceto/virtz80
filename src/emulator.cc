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
// core::Emulator
// ---------------------------------------------------------------------------

namespace core {

Emulator::Emulator()
    : base::Application("virtz80")
    , _vm(*this)
    , _duration(std::chrono::microseconds(16667))
    , _prev_time(ClockType::now())
    , _curr_time(ClockType::now())
    , _next_time(ClockType::now())
    , _turbo(Globals::turbo)
{
    _vm.reset();
}

auto Emulator::loop() -> void
{
    _prev_time = _curr_time;
    _curr_time = ClockType::now();
    _next_time = _next_time + _duration;
    if(_turbo != false) {
        _next_time = _curr_time;
    }
    else if(_curr_time < _next_time) {
        std::this_thread::sleep_until(_next_time);
        _curr_time = _next_time;
    }
    _vm.clock();
}

auto Emulator::quit() -> void
{
    if(_quit == false) {
        _quit = true;
        _vm.stop();
    }
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
