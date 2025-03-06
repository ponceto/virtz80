/*
 * globals.cc - Copyright (c) 2001-2025 - Olivier Poncet
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

// ---------------------------------------------------------------------------
// core::Globals
// ---------------------------------------------------------------------------

namespace core {

bool        Globals::verbose = false;
bool        Globals::turbo   = false;
std::string Globals::bank0;
std::string Globals::bank1;
std::string Globals::bank2;
std::string Globals::bank3;

}

// ---------------------------------------------------------------------------
// core::Globals
// ---------------------------------------------------------------------------

namespace core {

auto Globals::init() -> void
{
    if(bank0.empty()) {
        Globals::bank0 = "assets/zexall.rom";
    }
    if(bank1.empty()) {
        Globals::bank1 = "assets/bank1.rom";
    }
    if(bank2.empty()) {
        Globals::bank2 = "assets/bank2.rom";
    }
    if(bank3.empty()) {
        Globals::bank3 = "assets/bank3.rom";
    }
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
