/*
 * terminal.h - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifndef __APP_Terminal_h__
#define __APP_Terminal_h__

// ---------------------------------------------------------------------------
// aliases
// ---------------------------------------------------------------------------

namespace app {

using termios_type = struct termios;

}

// ---------------------------------------------------------------------------
// app::Terminal
// ---------------------------------------------------------------------------

namespace app {

class Terminal
{
public: // public interface
    Terminal(int fd0, int fd1, int fd2);

    Terminal(const Terminal&) = delete;

    Terminal& operator=(const Terminal&) = delete;

    virtual ~Terminal();

private: // private interface
    static auto get_attributes(int fd, termios_type& attributes) -> void;

    static auto set_attributes(int fd, termios_type& attributes) -> void;

private: // private data
    const int    _filedes[3];
    termios_type _termios[3];
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __APP_Terminal_h__ */
