/*
 * mmu-core.h - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifndef __DEV_MMU_CORE_H__
#define __DEV_MMU_CORE_H__

// ---------------------------------------------------------------------------
// forward declarations
// ---------------------------------------------------------------------------

namespace mmu {

class Instance;
class Interface;

}

// ---------------------------------------------------------------------------
// mmu::Bank
// ---------------------------------------------------------------------------

namespace mmu {

struct Bank
{
    uint8_t data[16384];
};

}

// ---------------------------------------------------------------------------
// mmu::State
// ---------------------------------------------------------------------------

namespace mmu {

struct State
{
    Bank bank[4];
};

}

// ---------------------------------------------------------------------------
// mmu::Instance
// ---------------------------------------------------------------------------

namespace mmu {

class Instance
{
public: // public interface
    Instance(Interface&);

    Instance(const Instance&) = delete;

    Instance& operator=(const Instance&) = delete;

    virtual ~Instance() = default;

    auto reset() -> void;

    auto clock() -> void;

    auto rd_byte(uint16_t addr, uint8_t data) -> uint8_t;

    auto wr_byte(uint16_t addr, uint8_t data) -> uint8_t;

    auto load_bank(const std::string& filename, const int index) -> void;

    auto save_bank(const std::string& filename, const int index) -> void;

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
// mmu::Interface
// ---------------------------------------------------------------------------

namespace mmu {

class Interface
{
public: // public interface
    Interface() = default;

    Interface(const Interface&) = default;

    Interface& operator=(const Interface&) = default;

    virtual ~Interface() = default;

    virtual auto mmu_char_wr(Instance&, uint8_t data) -> void = 0;
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __DEV_MMU_CORE_H__ */
