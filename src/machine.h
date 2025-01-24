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
#ifndef __CORE_Machine_h__
#define __CORE_Machine_h__

#include "emu/cpu/cpu-core.h"
#include "emu/mmu/mmu-core.h"
#include "emu/pmu/pmu-core.h"

// ---------------------------------------------------------------------------
// base::VirtualMachine
// ---------------------------------------------------------------------------

namespace base {

class VirtualMachine
{
public: // public interface
    VirtualMachine(const std::string& name);

    VirtualMachine(const VirtualMachine&) = delete;

    VirtualMachine& operator=(const VirtualMachine&) = delete;

    virtual ~VirtualMachine() = default;

    virtual auto main() -> void = 0;

    virtual auto stop() -> void = 0;

protected: // protected data
    const std::string _name;
    bool              _stop;
    FILE*             _istream;
    FILE*             _ostream;
};

}

// ---------------------------------------------------------------------------
// core::VirtualMachine
// ---------------------------------------------------------------------------

namespace core {

class VirtualMachine final
    : public base::VirtualMachine
    , private cpu::Interface
    , private mmu::Interface
    , private pmu::Interface
{
public: // public interface
    VirtualMachine();

    VirtualMachine(const VirtualMachine&) = delete;

    VirtualMachine& operator=(const VirtualMachine&) = delete;

    virtual ~VirtualMachine() = default;

    virtual auto main() -> void override final;

    virtual auto stop() -> void override final;

    auto reset() -> void;

    auto clock() -> void;

private: // private interface
    auto rd_char(int character = '\0') -> uint8_t;

    auto wr_char(int character = '\0') -> uint8_t;

private: // private cpu interface
    virtual auto cpu_mreq_m1(cpu::Instance& cpu, uint16_t addr, uint8_t data) -> uint8_t override final;

    virtual auto cpu_mreq_rd(cpu::Instance& cpu, uint16_t addr, uint8_t data) -> uint8_t override final;

    virtual auto cpu_mreq_wr(cpu::Instance& cpu, uint16_t addr, uint8_t data) -> uint8_t override final;

    virtual auto cpu_iorq_m1(cpu::Instance& cpu, uint16_t port, uint8_t data) -> uint8_t override final;

    virtual auto cpu_iorq_rd(cpu::Instance& cpu, uint16_t port, uint8_t data) -> uint8_t override final;

    virtual auto cpu_iorq_wr(cpu::Instance& cpu, uint16_t port, uint8_t data) -> uint8_t override final;

private: // private mmu interface
    virtual auto mmu_char_wr(mmu::Instance& mmu, uint8_t data) -> uint8_t override final;

private: // private pmu interface
    virtual auto pmu_ctrl_wr(pmu::Instance& pmu, uint8_t data) -> uint8_t override final;

private: // private data
    struct State
    {
        uint32_t clock = 4000000; /* master cpu clock */
        uint32_t hfreq = 15625;   /* horz. frequency  */
        uint32_t vfreq = 50;      /* vert. frequency  */
        uint32_t hcntr = 0;       /* horz. counter    */
        uint32_t vcntr = 0;       /* vert. counter    */
        uint32_t hltrq = 0;       /* halt request     */
        uint8_t  ichar = 0;       /* last input char  */
        uint8_t  ochar = 0;       /* last output char */
    };

private: // private data
    State         _state;
    cpu::Instance _cpu;
    mmu::Instance _mmu;
    pmu::Instance _pmu;
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __CORE_Machine_h__ */
