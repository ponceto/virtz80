/*
 * mmu-core.cc - Copyright (c) 2001-2025 - Olivier Poncet
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
#include "mmu-core.h"

// ---------------------------------------------------------------------------
// <anonymous>::mmu_io
// ---------------------------------------------------------------------------

namespace {

constexpr uint16_t MMU_OACK_ADDR = 0xfffd; /* output ack       */
constexpr uint16_t MMU_OREQ_ADDR = 0xfffe; /* output request   */
constexpr uint16_t MMU_OCHR_ADDR = 0xffff; /* output character */

}

// ---------------------------------------------------------------------------
// mmu::Instance
// ---------------------------------------------------------------------------

namespace mmu {

Instance::Instance(Interface& interface)
    : _interface(interface)
    , _state()
{
}

auto Instance::reset() -> void
{
    auto reset_bank = [&](Bank& bank) -> void
    {
        for(auto& byte : bank.data) {
            byte &= 0x00;
        }
    };

    auto reset_state = [&](State& state) -> void
    {
        for(auto& bank : state.bank) {
            reset_bank(bank);
        }
    };

    return reset_state(_state);
}

auto Instance::clock() -> void
{
}

auto Instance::rd_byte(uint16_t addr, uint8_t data) -> uint8_t
{
    const uint16_t bank_number = ((addr >> 14) & 0x0003);
    const uint16_t bank_offset = ((addr >>  0) & 0x3fff);

    return data = _state.bank[bank_number].data[bank_offset];
}

auto Instance::wr_byte(uint16_t addr, uint8_t data) -> uint8_t
{
    const uint16_t bank_number = ((addr >> 14) & 0x0003);
    const uint16_t bank_offset = ((addr >>  0) & 0x3fff);

    if(addr == MMU_OREQ_ADDR) {
        auto&    bank = _state.bank[bank_number];
        uint8_t& oack = bank.data[MMU_OACK_ADDR & 0x3fff];
        uint8_t& oreq = bank.data[MMU_OREQ_ADDR & 0x3fff];
        uint8_t& ochr = bank.data[MMU_OCHR_ADDR & 0x3fff];
        if(data != oreq) {
            static_cast<void>(_interface.mmu_char_wr(*this, ochr));
            ++oack;
        }
    }
    return _state.bank[bank_number].data[bank_offset] = data;
}

auto Instance::load_bank(const std::string& filename, const int index) -> void
{
    if((index >= 0) && (index <= 3)) {
        FILE* file = ::fopen(filename.c_str(), "rb");
        if(file != nullptr) {
            auto&            memory = _state.bank[index];
            uint8_t*         buffer = memory.data;
            constexpr size_t length = sizeof(memory.data);
            const     size_t result = ::fread(buffer, sizeof(*buffer), length, file);
            file = (::fclose(file), nullptr);
            if(result != length) {
                throw std::runtime_error("load_bank() has failed (load error)");
            }
        }
        else {
            throw std::runtime_error("load_bank() has failed (cannot open file)");
        }
    }
    else {
        throw std::runtime_error("load_bank() has failed (invalid index)");
    }
}

auto Instance::save_bank(const std::string& filename, const int index) -> void
{
    if((index >= 0) && (index <= 3)) {
        FILE* file = ::fopen(filename.c_str(), "wb");
        if(file != nullptr) {
            auto&            memory = _state.bank[index];
            uint8_t*         buffer = memory.data;
            constexpr size_t length = sizeof(memory.data);
            const     size_t result = ::fwrite(buffer, sizeof(*buffer), length, file);
            file = (::fclose(file), nullptr);
            if(result != length) {
                throw std::runtime_error("save_bank() has failed (save error)");
            }
        }
        else {
            throw std::runtime_error("save_bank() has failed (cannot open file)");
        }
    }
    else {
        throw std::runtime_error("save_bank() has failed (invalid index)");
    }
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
