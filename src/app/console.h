/*
 * console.h - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifndef __APP_Console_h__
#define __APP_Console_h__

// ---------------------------------------------------------------------------
// app::Console
// ---------------------------------------------------------------------------

namespace app {

struct Console
{
    static auto trace(const char* format, ...) -> void;

    static auto debug(const char* format, ...) -> void;

    static auto print(const char* format, ...) -> void;

    static auto alert(const char* format, ...) -> void;

    static auto error(const char* format, ...) -> void;

    static auto fatal(const char* format, ...) -> void;
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __APP_Console_h__ */
