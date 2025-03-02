/*
 * program.h - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifndef __CORE_Program_h__
#define __CORE_Program_h__

// ---------------------------------------------------------------------------
// core::ArgList
// ---------------------------------------------------------------------------

namespace core {

using ArgList = std::vector<std::string>;

}

// ---------------------------------------------------------------------------
// core::Program
// ---------------------------------------------------------------------------

namespace core {

struct Program
{
    static auto init(const ArgList& args) -> bool;

    static auto main(const ArgList& args) -> void;

    static auto help(const ArgList& args) -> void;
};

}

// ---------------------------------------------------------------------------
// core::Terminal
// ---------------------------------------------------------------------------

namespace core {

class Terminal
{
public: // public interface
    Terminal();

    Terminal(const Terminal&) = delete;

    Terminal& operator=(const Terminal&) = delete;

    virtual ~Terminal();

private: // private interface
    using termios_type = struct termios;

    auto get_attributes(int fd, termios_type& attributes) -> void;

    auto set_attributes(int fd, termios_type& attributes) -> void;

private: // private data
    termios_type _attributes[3];
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __CORE_Program_h__ */
