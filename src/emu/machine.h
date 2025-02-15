/*
 * machine.h - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifndef __EMU_Machine_h__
#define __EMU_Machine_h__

#include "cpu/cpu-core.h"
#include "mmu/mmu-core.h"
#include "pmu/pmu-core.h"

// ---------------------------------------------------------------------------
// forward declarations
// ---------------------------------------------------------------------------

namespace emu {

class MachineInstance;
class MachineInterface;

}

// ---------------------------------------------------------------------------
// emu::MachineInstance
// ---------------------------------------------------------------------------

namespace emu {

class MachineInstance final
    : private cpu::Interface
    , private mmu::Interface
    , private pmu::Interface
{
public: // public interface
    MachineInstance(MachineInterface&);

    MachineInstance(const MachineInstance&) = delete;

    MachineInstance& operator=(const MachineInstance&) = delete;

    virtual ~MachineInstance();

    auto reset() -> void;

    auto clock() -> void;

private: // private interface
    auto rd_char(int character = '\0') -> uint8_t;

    auto wr_char(int character = '\0') -> uint8_t;

private: // private cpu interface
    virtual auto cpu_mreq_m1(cpu::Instance&, uint16_t addr, uint8_t data) -> uint8_t override final;

    virtual auto cpu_mreq_rd(cpu::Instance&, uint16_t addr, uint8_t data) -> uint8_t override final;

    virtual auto cpu_mreq_wr(cpu::Instance&, uint16_t addr, uint8_t data) -> uint8_t override final;

    virtual auto cpu_iorq_m1(cpu::Instance&, uint16_t port, uint8_t data) -> uint8_t override final;

    virtual auto cpu_iorq_rd(cpu::Instance&, uint16_t port, uint8_t data) -> uint8_t override final;

    virtual auto cpu_iorq_wr(cpu::Instance&, uint16_t port, uint8_t data) -> uint8_t override final;

private: // private mmu interface
    virtual auto mmu_char_wr(mmu::Instance&, uint8_t data) -> uint8_t override final;

private: // private pmu interface
    virtual auto pmu_ctrl_wr(pmu::Instance&, uint8_t data) -> uint8_t override final;

private: // private types
    struct State
    {
        uint32_t clock   = 4000000; /* master cpu clock  */
        uint32_t hfreq   = 15625;   /* horz. frequency   */
        uint32_t vfreq   = 50;      /* vert. frequency   */
        uint32_t hcntr   = 0;       /* horz. counter     */
        uint32_t vcntr   = 0;       /* vert. counter     */
        uint32_t hltrq   = 0;       /* halt request      */
        uint8_t  ichar   = 0;       /* last input char   */
        uint8_t  ochar   = 0;       /* last output char  */
        bool     stopped = false;   /* emulation stopped */
    };

private: // private data
    MachineInterface& _interface;
    FILE*             _istream;
    FILE*             _ostream;
    State             _state;
    cpu::Instance     _cpu;
    mmu::Instance     _mmu;
    pmu::Instance     _pmu;
};

}

// ---------------------------------------------------------------------------
// emu::MachineInterface
// ---------------------------------------------------------------------------

namespace emu {

class MachineInterface
{
public: // public interface
    MachineInterface() = default;

    MachineInterface(const MachineInterface&) = default;

    MachineInterface& operator=(const MachineInterface&) = default;

    virtual ~MachineInterface() = default;

    virtual auto quit() -> void = 0;
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __EMU_Machine_h__ */
