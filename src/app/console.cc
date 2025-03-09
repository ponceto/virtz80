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
#include <chrono>
#include <thread>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "console.h"

// ---------------------------------------------------------------------------
// app::Console
// ---------------------------------------------------------------------------

namespace app {

auto Console::trace(const char* format, ...) -> void
{
    auto va_trace = [](FILE* stream, const char* format, va_list arguments) -> void
    {
        static_cast<void>(::fputc('T', stream));
        static_cast<void>(::fputc('\t', stream));
        static_cast<void>(::vfprintf(stream, format, arguments));
        static_cast<void>(::fputc('\n', stream));
        static_cast<void>(::fflush(stream));
    };

    va_list arguments;
    va_start(arguments, format);
    va_trace(stdout, format, arguments);
    va_end(arguments);
}

auto Console::debug(const char* format, ...) -> void
{
    auto va_debug = [](FILE* stream, const char* format, va_list arguments) -> void
    {
        static_cast<void>(::fputc('D', stream));
        static_cast<void>(::fputc('\t', stream));
        static_cast<void>(::vfprintf(stream, format, arguments));
        static_cast<void>(::fputc('\n', stream));
        static_cast<void>(::fflush(stream));
    };

    va_list arguments;
    va_start(arguments, format);
    va_debug(stdout, format, arguments);
    va_end(arguments);
}

auto Console::print(const char* format, ...) -> void
{
    auto va_print = [](FILE* stream, const char* format, va_list arguments) -> void
    {
        static_cast<void>(::fputc('I', stream));
        static_cast<void>(::fputc('\t', stream));
        static_cast<void>(::vfprintf(stream, format, arguments));
        static_cast<void>(::fputc('\n', stream));
        static_cast<void>(::fflush(stream));
    };

    va_list arguments;
    va_start(arguments, format);
    va_print(stdout, format, arguments);
    va_end(arguments);
}

auto Console::alert(const char* format, ...) -> void
{
    auto va_alert = [](FILE* stream, const char* format, va_list arguments) -> void
    {
        static_cast<void>(::fputc('W', stream));
        static_cast<void>(::fputc('\t', stream));
        static_cast<void>(::vfprintf(stream, format, arguments));
        static_cast<void>(::fputc('\n', stream));
        static_cast<void>(::fflush(stream));
    };

    va_list arguments;
    va_start(arguments, format);
    va_alert(stderr, format, arguments);
    va_end(arguments);
}

auto Console::error(const char* format, ...) -> void
{
    auto va_error = [](FILE* stream, const char* format, va_list arguments) -> void
    {
        static_cast<void>(::fputc('E', stream));
        static_cast<void>(::fputc('\t', stream));
        static_cast<void>(::vfprintf(stream, format, arguments));
        static_cast<void>(::fputc('\n', stream));
        static_cast<void>(::fflush(stream));
    };

    va_list arguments;
    va_start(arguments, format);
    va_error(stderr, format, arguments);
    va_end(arguments);
}

auto Console::fatal(const char* format, ...) -> void
{
    auto va_fatal = [](FILE* stream, const char* format, va_list arguments) -> void
    {
        static_cast<void>(::fputc('F', stream));
        static_cast<void>(::fputc('\t', stream));
        static_cast<void>(::vfprintf(stream, format, arguments));
        static_cast<void>(::fputc('\n', stream));
        static_cast<void>(::fflush(stream));
    };

    va_list arguments;
    va_start(arguments, format);
    va_fatal(stderr, format, arguments);
    va_end(arguments);
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
