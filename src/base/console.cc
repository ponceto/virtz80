/*
 * console.cc - Copyright (c) 2001-2025 - Olivier Poncet
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
#include "console.h"

// ---------------------------------------------------------------------------
// base::Console
// ---------------------------------------------------------------------------

namespace base {

auto Console::trace(const char* format, ...) -> void
{
    FILE* stream(stdout);

    va_list arguments;
    va_start(arguments, format);
    static_cast<void>(::fputc('T', stream));
    static_cast<void>(::fputc('\t', stream));
    static_cast<void>(::vfprintf(stream, format, arguments));
    static_cast<void>(::fputc('\n', stream));
    va_end(arguments);
}

auto Console::debug(const char* format, ...) -> void
{
    FILE* stream(stdout);

    va_list arguments;
    va_start(arguments, format);
    static_cast<void>(::fputc('D', stream));
    static_cast<void>(::fputc('\t', stream));
    static_cast<void>(::vfprintf(stream, format, arguments));
    static_cast<void>(::fputc('\n', stream));
    va_end(arguments);
}

auto Console::print(const char* format, ...) -> void
{
    FILE* stream(stdout);

    va_list arguments;
    va_start(arguments, format);
    static_cast<void>(::fputc('I', stream));
    static_cast<void>(::fputc('\t', stream));
    static_cast<void>(::vfprintf(stream, format, arguments));
    static_cast<void>(::fputc('\n', stream));
    va_end(arguments);
}

auto Console::alert(const char* format, ...) -> void
{
    FILE* stream(stderr);

    va_list arguments;
    va_start(arguments, format);
    static_cast<void>(::fputc('W', stream));
    static_cast<void>(::fputc('\t', stream));
    static_cast<void>(::vfprintf(stream, format, arguments));
    static_cast<void>(::fputc('\n', stream));
    va_end(arguments);
}

auto Console::error(const char* format, ...) -> void
{
    FILE* stream(stderr);

    va_list arguments;
    va_start(arguments, format);
    static_cast<void>(::fputc('E', stream));
    static_cast<void>(::fputc('\t', stream));
    static_cast<void>(::vfprintf(stream, format, arguments));
    static_cast<void>(::fputc('\n', stream));
    va_end(arguments);
}

auto Console::fatal(const char* format, ...) -> void
{
    FILE* stream(stderr);

    va_list arguments;
    va_start(arguments, format);
    static_cast<void>(::fputc('F', stream));
    static_cast<void>(::fputc('\t', stream));
    static_cast<void>(::vfprintf(stream, format, arguments));
    static_cast<void>(::fputc('\n', stream));
    va_end(arguments);
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
