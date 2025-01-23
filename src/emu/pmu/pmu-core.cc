/*
 * pmu-core.cc - Copyright (c) 2001-2025 - Olivier Poncet
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
#include "pmu-core.h"

// ---------------------------------------------------------------------------
// <anonymous>::pmu_io
// ---------------------------------------------------------------------------

namespace {

constexpr uint16_t PMU_CTRL_PORT = 0x0001;

}

// ---------------------------------------------------------------------------
// pmu::Instance
// ---------------------------------------------------------------------------

namespace pmu {

Instance::Instance(Interface& interface)
    : _interface(interface)
    , _state()
{
}

auto Instance::reset() -> void
{
    auto reset_state = [&](State& state) -> void
    {
        state.reserved &= 0;
    };

    return reset_state(_state);
}

auto Instance::clock() -> void
{
}

auto Instance::rd_byte(uint16_t port, uint8_t data) -> uint8_t
{
    return data;
}

auto Instance::wr_byte(uint16_t port, uint8_t data) -> uint8_t
{
    if(port == PMU_CTRL_PORT) {
        static_cast<void>(_interface.pmu_ctrl_wr(*this, data));
    }
    return data;
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
