/*
 * virtual-machine.h - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifndef __EMU_VirtualMachine_h__
#define __EMU_VirtualMachine_h__

#include "dev/cpu/cpu-core.h"
#include "dev/mmu/mmu-core.h"
#include "dev/vdu/vdu-core.h"
#include "dev/sio/sio-core.h"

// ---------------------------------------------------------------------------
// forward declarations
// ---------------------------------------------------------------------------

namespace emu {

class VirtualMachine;
class VirtualMachineIface;

}

// ---------------------------------------------------------------------------
// emu::VirtualMachineState
// ---------------------------------------------------------------------------

namespace emu {

struct VirtualMachineState
{
    uint32_t cpu_clock = 7372800; /* cpu clock (RC2014) */
    uint32_t cpu_ticks = 0;       /* cpu ticks          */
    uint32_t vdu_clock = 4134375; /* vdu clock (NTSC)   */
    uint32_t vdu_ticks = 0;       /* vdu ticks          */
    uint32_t sio_clock = 115200;  /* sio clock (ACIA)   */
    uint32_t sio_ticks = 0;       /* sio ticks          */
    uint32_t max_clock = 0;       /* max clock          */
    uint32_t hlt_count = 0;       /* halt request       */
    uint32_t wdt_count = 0;       /* watchdog           */
    bool     stopped   = false;   /* emulation stopped  */
    bool     ready     = false;   /* a frame is ready   */
};

}

// ---------------------------------------------------------------------------
// emu::VirtualMachine
// ---------------------------------------------------------------------------

namespace emu {

class VirtualMachine final
    : private cpu::Interface
    , private mmu::Interface
    , private vdu::Interface
    , private sio::Interface
{
public: // public interface
    VirtualMachine(VirtualMachineIface&);

    VirtualMachine(const VirtualMachine&) = delete;

    VirtualMachine& operator=(const VirtualMachine&) = delete;

    virtual ~VirtualMachine();

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
    virtual auto mmu_char_wr(mmu::Instance&, uint8_t data) -> void override final;

private: // private vdu interface
    virtual auto vdu_sync_hs(vdu::Instance&, bool hsync) -> void override final;

    virtual auto vdu_sync_vs(vdu::Instance&, bool vsync) -> void override final;

private: // private sio interface
    virtual auto sio_intr_rq(sio::Instance&) -> void override final;

private: // private data
    VirtualMachineIface& _iface;
    VirtualMachineState  _state;
    cpu::Instance        _cpu;
    mmu::Instance        _mmu;
    vdu::Instance        _vdu;
    sio::Instance        _sio0;
    sio::Instance        _sio1;
};

}

// ---------------------------------------------------------------------------
// emu::VirtualMachineIface
// ---------------------------------------------------------------------------

namespace emu {

class VirtualMachineIface
{
public: // public interface
    VirtualMachineIface() = default;

    VirtualMachineIface(const VirtualMachineIface&) = default;

    VirtualMachineIface& operator=(const VirtualMachineIface&) = default;

    virtual ~VirtualMachineIface() = default;

    virtual auto loop() -> void = 0;

    virtual auto quit() -> void = 0;

    virtual auto get(const std::string& name) -> std::string = 0;
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __EMU_VirtualMachine_h__ */
