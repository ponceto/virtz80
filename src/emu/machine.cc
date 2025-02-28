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
    , _setup()
    , _state()
    , _istream(stdin)
    , _ostream(stdout)
    , _cpu(*this)
    , _mmu(*this)
    , _pmu(*this)
    , _vdu(*this)
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
        _state.cpu_clock |= 0;
        _state.cpu_ticks &= 0;
        _state.vdu_clock |= 0;
        _state.vdu_ticks &= 0;
        _state.max_clock &= 0;
        _state.hlt_req   &= 0;
        _state.ichar     &= 0;
        _state.ochar     &= 0;
        _state.ready     &= false;
        _state.stopped   &= false;

        if(_state.cpu_clock >= _state.max_clock) {
            _state.max_clock = _state.cpu_clock;
        }
        if(_state.vdu_clock >= _state.max_clock) {
            _state.max_clock = _state.vdu_clock;
        }
    };

    auto reset_cpu = [&]() -> void
    {
        _cpu.reset();
    };

    auto reset_mmu = [&]() -> void
    {
        _mmu.reset();
        _mmu.load_bank(_setup.bank0, 0);
        _mmu.load_bank(_setup.bank1, 1);
        _mmu.load_bank(_setup.bank2, 2);
        _mmu.load_bank(_setup.bank3, 3);
    };

    auto reset_pmu = [&]() -> void
    {
        _pmu.reset();
    };

    auto reset_vdu = [&]() -> void
    {
        _vdu.reset();
    };

    auto reset_all = [&]() -> void
    {
        reset_state();
        reset_cpu();
        reset_mmu();
        reset_pmu();
        reset_vdu();
    };

    return reset_all();
}

auto MachineInstance::clock() -> void
{
    if((_state.ready = _state.stopped) == false) {
        const uint32_t cpu_clock = _state.cpu_clock;
        const uint32_t vdu_clock = _state.vdu_clock;
        const uint32_t max_clock = _state.max_clock;
        do {
            if((_state.cpu_ticks += cpu_clock) >= max_clock) {
                _state.cpu_ticks -= max_clock;
                _cpu.clock();
            }
            if((_state.vdu_ticks += vdu_clock) >= max_clock) {
                _state.vdu_ticks -= max_clock;
                _vdu.clock();
            }
        } while((_state.ready |= _state.stopped) == false);
    }
}

auto MachineInstance::stop() -> void
{
    if(_state.stopped == false) {
        _state.stopped = true;
        _interface.quit();
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
#ifdef __EMSCRIPTEN__
    if(character == '\r') {
        return character;
    }
#endif
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
        if(++_state.hlt_req >= 2) {
            stop();
        }
    }
    return data;
}

auto MachineInstance::vdu_sync_hs(vdu::Instance& vdu, bool data) -> void
{
    _state.ready |= false;
}

auto MachineInstance::vdu_sync_vs(vdu::Instance& vdu, bool data) -> void
{
    _state.ready |= true;
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
