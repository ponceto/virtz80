/*
 * vdu-core.cc - Copyright (c) 2001-2025 - Olivier Poncet
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
#include "vdu-core.h"

// ---------------------------------------------------------------------------
// vdu::Instance
// ---------------------------------------------------------------------------

namespace vdu {

Instance::Instance(Interface& interface)
    : _interface(interface)
    , _state()
{
}

auto Instance::reset() -> void
{
    auto reset_state = [&](State& state) -> void
    {
        state.clock |= 0; // don't reset
        state.hfreq |= 0; // don't reset
        state.vfreq |= 0; // don't reset
        state.hcntr &= 0; // reset value
        state.vcntr &= 0; // reset value
    };

    return reset_state(_state);
}

auto Instance::clock() -> void
{
    if((_state.hcntr += _state.hfreq) >= _state.clock) {
        _state.hcntr -= _state.clock;
        _interface.vdu_sync_hs(*this, false);
    }
    if((_state.vcntr += _state.vfreq) >= _state.clock) {
        _state.vcntr -= _state.clock;
        _interface.vdu_sync_vs(*this, false);
    }
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
