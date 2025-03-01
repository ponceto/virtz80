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
    , _cpu(*this)
    , _mmu(*this)
    , _vdu(*this)
    , _sio(*this)
{
}

MachineInstance::~MachineInstance()
{
    _sio.print('\n');
}

auto MachineInstance::reset() -> void
{
    auto reset_state = [&]() -> void
    {
        _state.cpu_clock |= 0;
        _state.cpu_ticks &= 0;
        _state.vdu_clock |= 0;
        _state.vdu_ticks &= 0;
        _state.sio_clock |= 0;
        _state.sio_ticks &= 0;
        _state.max_clock &= 0;
        _state.hlt_req   &= 0;
        _state.ready     &= false;
        _state.stopped   &= false;

        if(_state.cpu_clock >= _state.max_clock) {
            _state.max_clock = _state.cpu_clock;
        }
        if(_state.vdu_clock >= _state.max_clock) {
            _state.max_clock = _state.vdu_clock;
        }
        if(_state.sio_clock >= _state.max_clock) {
            _state.max_clock = _state.sio_clock;
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

    auto reset_vdu = [&]() -> void
    {
        _vdu.reset();
    };

    auto reset_sio = [&]() -> void
    {
        _sio.reset();
    };

    auto reset_all = [&]() -> void
    {
        reset_state();
        reset_cpu();
        reset_mmu();
        reset_vdu();
        reset_sio();
    };

    return reset_all();
}

auto MachineInstance::clock() -> void
{
    if((_state.ready = _state.stopped) == false) {
        const uint32_t cpu_clock = _state.cpu_clock;
        const uint32_t vdu_clock = _state.vdu_clock;
        const uint32_t sio_clock = _state.sio_clock;
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
            if((_state.sio_ticks += sio_clock) >= max_clock) {
                _state.sio_ticks -= max_clock;
                _sio.clock();
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
    return 0x00;
}

auto MachineInstance::cpu_iorq_rd(cpu::Instance& cpu, uint16_t port, uint8_t data) -> uint8_t
{
    if(port == 0x0001) {
        data = 0xff;
    }
    if((port & 0x0080) != 0) {
        if((port & 0x0001) != 0) {
            data = _sio.rd_acia_data(data);
        }
        else {
            data = _sio.rd_acia_stat(data);
        }
    }
    return data;
}

auto MachineInstance::cpu_iorq_wr(cpu::Instance& cpu, uint16_t port, uint8_t data) -> uint8_t
{
    if(port == 0x0001) {
        if(data == 0x00) {
            if(++_state.hlt_req >= 2) {
                stop();
            }
        }
    }
    if((port & 0x0080) != 0) {
        if((port & 0x0001) != 0) {
            data = _sio.wr_acia_data(data);
        }
        else {
            data = _sio.wr_acia_ctrl(data);
        }
    }
    return data;
}

auto MachineInstance::mmu_char_wr(mmu::Instance& mmu, uint8_t data) -> uint8_t
{
    return _sio.print(data);
}

auto MachineInstance::vdu_sync_hs(vdu::Instance& vdu, bool data) -> void
{
    _state.ready |= false;
}

auto MachineInstance::vdu_sync_vs(vdu::Instance& vdu, bool data) -> void
{
    _state.ready |= true;
}

auto MachineInstance::sio_intr_rq(sio::Instance& sio) -> void
{
    _cpu.pulse_int();
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
