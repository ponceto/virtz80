/*
 * machine.cc - Copyright (c) 2001-2025 - Olivier Poncet
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
#include "globals.h"
#include "machine.h"

// ---------------------------------------------------------------------------
// emu::MachineInstance
// ---------------------------------------------------------------------------

namespace emu {

MachineInstance::MachineInstance(MachineInterface& interface)
    : _interface(interface)
    , _istream(stdin)
    , _ostream(stdout)
    , _state()
    , _cpu(*this)
    , _mmu(*this)
    , _pmu(*this)
{
}

MachineInstance::~MachineInstance()
{
    if((_state.ochar != '\0') && (_state.ochar != '\n')) {
        static_cast<void>(wr_char('\n'));
    }
}

auto MachineInstance::reset() -> void
{
    auto reset_state = [&]() -> void
    {
        _state.clock   |= 0;     // don't reset
        _state.hfreq   |= 0;     // don't reset
        _state.vfreq   |= 0;     // don't reset
        _state.hcntr   &= 0;     // reset value
        _state.vcntr   &= 0;     // reset value
        _state.hltrq   &= 0;     // reset value
        _state.ichar   &= 0;     // reset value
        _state.ochar   &= 0;     // reset value
        _state.stopped &= false; // reset value
    };

    auto reset_cpu = [&]() -> void
    {
        _cpu.reset();
    };

    auto reset_mmu = [&]() -> void
    {
        _mmu.reset();
        _mmu.load_bank(core::Globals::bank0, 0);
        _mmu.load_bank(core::Globals::bank1, 1);
        _mmu.load_bank(core::Globals::bank2, 2);
        _mmu.load_bank(core::Globals::bank3, 3);
    };

    auto reset_pmu = [&]() -> void
    {
        _pmu.reset();
    };

    auto reset_all = [&]() -> void
    {
        reset_state();
        reset_cpu();
        reset_mmu();
        reset_pmu();
    };

    return reset_all();
}

auto MachineInstance::clock() -> void
{
    bool done = false;

    if((done |= _state.stopped) == false) {
        do {
            _cpu.clock();
            if((_state.hcntr += _state.hfreq) >= _state.clock) {
                _state.hcntr -= _state.clock;
                done |= false;
            }
            if((_state.vcntr += _state.vfreq) >= _state.clock) {
                _state.vcntr -= _state.clock;
                done |= true;
            }
        } while((done |= _state.stopped) == false);
    }
}

auto MachineInstance::rd_char(int character) -> uint8_t
{
    if((character = ::fgetc(_istream)) != EOF) {
    //  static_cast<void>(::fflush(_istream));
        _state.ichar = character;
    }
    else {
        _state.ichar = '\0';
    }
    return _state.ichar;
}

auto MachineInstance::wr_char(int character) -> uint8_t
{
    if((character = ::fputc(character, _ostream)) != EOF) {
        static_cast<void>(::fflush(_ostream));
        _state.ochar = character;
    }
    else {
        _state.ochar = '\0';
    }
    return _state.ochar;
}

auto MachineInstance::cpu_mreq_m1(cpu::Instance& cpu, uint16_t addr, uint8_t data) -> uint8_t
{
    return _mmu.rd_byte(addr, data);
}

auto MachineInstance::cpu_mreq_rd(cpu::Instance& cpu, uint16_t addr, uint8_t data) -> uint8_t
{
    return _mmu.rd_byte(addr, data);
}

auto MachineInstance::cpu_mreq_wr(cpu::Instance& cpu, uint16_t addr, uint8_t data) -> uint8_t
{
    return _mmu.wr_byte(addr, data);
}

auto MachineInstance::cpu_iorq_m1(cpu::Instance& cpu, uint16_t port, uint8_t data) -> uint8_t
{
    return _pmu.rd_byte(port, data);
}

auto MachineInstance::cpu_iorq_rd(cpu::Instance& cpu, uint16_t port, uint8_t data) -> uint8_t
{
    return _pmu.rd_byte(port, data);
}

auto MachineInstance::cpu_iorq_wr(cpu::Instance& cpu, uint16_t port, uint8_t data) -> uint8_t
{
    return _pmu.wr_byte(port, data);
}

auto MachineInstance::mmu_char_wr(mmu::Instance& mmu, uint8_t data) -> uint8_t
{
    static_cast<void>(wr_char(data));

    return data;
}

auto MachineInstance::pmu_ctrl_wr(pmu::Instance& pmu, uint8_t data) -> uint8_t
{
    if(data == 0x00) {
        if(++_state.hltrq >= 2) {
            _state.stopped = true;
            _interface.quit();
        }
    }
    return data;
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
