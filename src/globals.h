/*
 * globals.h - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifndef __CORE_Globals_h__
#define __CORE_Globals_h__

// ---------------------------------------------------------------------------
// core::Globals
// ---------------------------------------------------------------------------

namespace core {

struct Globals
{
    static auto init() -> void;

    static bool        turbo;
    static std::string bank0;
    static std::string bank1;
    static std::string bank2;
    static std::string bank3;
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __CORE_Globals_h__ */
