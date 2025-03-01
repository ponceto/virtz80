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
#include "vdu/vdu-core.h"
#include "sio/sio-core.h"

// ---------------------------------------------------------------------------
// forward declarations
// ---------------------------------------------------------------------------

namespace emu {

class MachineInstance;
class MachineInterface;

}

// ---------------------------------------------------------------------------
// emu::MachineSetup
// ---------------------------------------------------------------------------

namespace emu {

struct MachineSetup
{
    std::string bank0 = core::Globals::bank0;
    std::string bank1 = core::Globals::bank1;
    std::string bank2 = core::Globals::bank2;
    std::string bank3 = core::Globals::bank3;
};

}

// ---------------------------------------------------------------------------
// emu::MachineState
// ---------------------------------------------------------------------------

namespace emu {

struct MachineState
{
    uint32_t cpu_clock = 7372800; /* cpu clock (RC2014) */
    uint32_t cpu_ticks = 0;       /* cpu ticks          */
    uint32_t vdu_clock = 4134375; /* vdu clock (NTSC)   */
    uint32_t vdu_ticks = 0;       /* vdu ticks          */
    uint32_t sio_clock = 115200;  /* sio clock (SIO)    */
    uint32_t sio_ticks = 0;       /* sio ticks          */
    uint32_t max_clock = 0;       /* max clock          */
    uint32_t hlt_req   = 0;       /* halt request       */
    bool     ready     = false;   /* a frame is ready   */
    bool     stopped   = false;   /* emulation stopped  */
};

}

// ---------------------------------------------------------------------------
// emu::MachineInstance
// ---------------------------------------------------------------------------

namespace emu {

class MachineInstance final
    : private cpu::Interface
    , private mmu::Interface
    , private vdu::Interface
    , private sio::Interface
{
public: // public interface
    MachineInstance(MachineInterface&);

    MachineInstance(const MachineInstance&) = delete;

    MachineInstance& operator=(const MachineInstance&) = delete;

    virtual ~MachineInstance();

    auto reset() -> void;

    auto clock() -> void;

    auto stop() -> void;

private: // private cpu interface
    virtual auto cpu_mreq_m1(cpu::Instance&, uint16_t addr, uint8_t data) -> uint8_t override final;

    virtual auto cpu_mreq_rd(cpu::Instance&, uint16_t addr, uint8_t data) -> uint8_t override final;

    virtual auto cpu_mreq_wr(cpu::Instance&, uint16_t addr, uint8_t data) -> uint8_t override final;

    virtual auto cpu_iorq_m1(cpu::Instance&, uint16_t port, uint8_t data) -> uint8_t override final;

    virtual auto cpu_iorq_rd(cpu::Instance&, uint16_t port, uint8_t data) -> uint8_t override final;

    virtual auto cpu_iorq_wr(cpu::Instance&, uint16_t port, uint8_t data) -> uint8_t override final;

private: // private mmu interface
    virtual auto mmu_char_wr(mmu::Instance&, uint8_t data) -> uint8_t override final;

private: // private vdu interface
    virtual auto vdu_sync_hs(vdu::Instance&, bool hsync) -> void override final;

    virtual auto vdu_sync_vs(vdu::Instance&, bool vsync) -> void override final;

private: // private sio interface
    virtual auto sio_intr_rq(sio::Instance&) -> void override final;

private: // private data
    MachineInterface& _interface;
    MachineSetup      _setup;
    MachineState      _state;
    cpu::Instance     _cpu;
    mmu::Instance     _mmu;
    vdu::Instance     _vdu;
    sio::Instance     _sio;
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
