/*
 * cpu-core.h - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifndef __EMU_CPU_CORE_H__
#define __EMU_CPU_CORE_H__

// ---------------------------------------------------------------------------
// forward declarations
// ---------------------------------------------------------------------------

namespace cpu {

class Instance;
class Interface;

}

// ---------------------------------------------------------------------------
// cpu::Register
// ---------------------------------------------------------------------------

namespace cpu {

union Register
{
    struct /* long */
    {
        uint32_t r;
    } l;
#if defined(MSB_FIRST) && !defined(LSB_FIRST)
    struct /* word */
    {
        uint16_t h;
        uint16_t l;
    } w;
    struct /* byte */
    {
        uint8_t  x;
        uint8_t  y;
        uint8_t  h;
        uint8_t  l;
    } b;
#endif
#if !defined(MSB_FIRST) && defined(LSB_FIRST)
    struct /* word */
    {
        uint16_t l;
        uint16_t h;
    } w;
    struct /* byte */
    {
        uint8_t  l;
        uint8_t  h;
        uint8_t  y;
        uint8_t  x;
    } b;
#endif
};

}

// ---------------------------------------------------------------------------
// cpu::State
// ---------------------------------------------------------------------------

namespace cpu {

struct State
{
    Register r_af;     /* AF & AF'            */
    Register r_bc;     /* BC & BC'            */
    Register r_de;     /* DE & DE'            */
    Register r_hl;     /* HL & HL'            */
    Register r_ix;     /* IX Index            */
    Register r_iy;     /* IY Index            */
    Register r_sp;     /* Stack Pointer       */
    Register r_pc;     /* Program Counter     */
    Register r_ir;     /* Interrupt & Refresh */
    Register r_st;     /* IFF, IM & Control   */
    uint32_t m_cycles; /* M-Cycles            */
    uint32_t t_states; /* T-States            */
    uint32_t i_period; /* I-Period            */
};

}

// ---------------------------------------------------------------------------
// cpu::Instance
// ---------------------------------------------------------------------------

namespace cpu {

class Instance
{
public: // public interface
    Instance(Interface&);

    Instance(const Instance&) = delete;

    Instance& operator=(const Instance&) = delete;

    virtual ~Instance() = default;

    auto reset() -> void;

    auto clock() -> void;

    auto pulse_nmi() -> void;

    auto pulse_int() -> void;

    auto operator->() -> State*
    {
        return &_state;
    }

protected: // protected data
    Interface& _interface;
    State      _state;
};

}

// ---------------------------------------------------------------------------
// cpu::Interface
// ---------------------------------------------------------------------------

namespace cpu {

class Interface
{
public: // public interface
    Interface() = default;

    Interface(const Interface&) = default;

    Interface& operator=(const Interface&) = default;

    virtual ~Interface() = default;

    virtual auto cpu_mreq_m1(Instance&, uint16_t addr, uint8_t data) -> uint8_t = 0;

    virtual auto cpu_mreq_rd(Instance&, uint16_t addr, uint8_t data) -> uint8_t = 0;

    virtual auto cpu_mreq_wr(Instance&, uint16_t addr, uint8_t data) -> uint8_t = 0;

    virtual auto cpu_iorq_m1(Instance&, uint16_t port, uint8_t data) -> uint8_t = 0;

    virtual auto cpu_iorq_rd(Instance&, uint16_t port, uint8_t data) -> uint8_t = 0;

    virtual auto cpu_iorq_wr(Instance&, uint16_t port, uint8_t data) -> uint8_t = 0;
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __EMU_CPU_CORE_H__ */
