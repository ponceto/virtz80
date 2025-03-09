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
#ifndef __APP_Emulator_h__
#define __APP_Emulator_h__

#include "application.h"
#include "emu/virtual-machine.h"

// ---------------------------------------------------------------------------
// aliases
// ---------------------------------------------------------------------------

namespace app {

using VirtualMachine      = emu::VirtualMachine;
using VirtualMachineIface = emu::VirtualMachineIface;

}

// ---------------------------------------------------------------------------
// app::Emulator
// ---------------------------------------------------------------------------

namespace app {

class Emulator final
    : public Application
    , public VirtualMachineIface
{
public: // public interface
    Emulator();

    Emulator(const Emulator&) = delete;

    Emulator& operator=(const Emulator&) = delete;

    virtual ~Emulator() = default;

    virtual auto loop() -> void override final;

    virtual auto quit() -> void override final;

    virtual auto get(const std::string& name) -> std::string override final;

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

#endif /* __APP_Emulator_h__ */
