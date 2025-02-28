/*
 * emulator.h - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifndef __CORE_Emulator_h__
#define __CORE_Emulator_h__

#include "base/console.h"
#include "base/application.h"
#include "emu/machine.h"

// ---------------------------------------------------------------------------
// aliases
// ---------------------------------------------------------------------------

namespace core {

using VirtualMachine      = emu::MachineInstance;
using VirtualMachineIface = emu::MachineInterface;

}

// ---------------------------------------------------------------------------
// core::Emulator
// ---------------------------------------------------------------------------

namespace core {

class Emulator final
    : public base::Application
    , private VirtualMachineIface
{
public: // public interface
    Emulator();

    Emulator(const Emulator&) = delete;

    Emulator& operator=(const Emulator&) = delete;

    virtual ~Emulator() = default;

    virtual auto loop() -> void override final;

    virtual auto quit() -> void override final;

private: // private data
    using ClockType     = std::chrono::steady_clock;
    using DurationType  = ClockType::duration;
    using TimePointType = ClockType::time_point;

private: // private data
    VirtualMachine _vm;
    DurationType   _duration;
    TimePointType  _prev_time;
    TimePointType  _curr_time;
    TimePointType  _next_time;
    bool           _turbo;
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __CORE_Emulator_h__ */
