/*
 * program.cc - Copyright (c) 2001-2025 - Olivier Poncet
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
#include <locale>
#include <memory>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "console.h"
#include "globals.h"
#include "machine.h"
#include "program.h"

// ---------------------------------------------------------------------------
// core::Program
// ---------------------------------------------------------------------------

namespace core {

namespace detail {

auto program_name(const ArgList& args) -> const char*
{
    const char* arg = args[0].c_str();
    const char* sep = ::strrchr(arg, '/');
    if(sep != nullptr) {
        arg = sep + 1;
    }
    return arg;
}

auto arg_is(const std::string& argument, const std::string& expected) -> bool
{
    bool        res = false;
    const char* exp = expected.c_str();
    const char* arg = argument.c_str();
    const char* equ = ::strchr(arg, '=');
    if(equ == nullptr) {
        res = (::strcmp(exp, arg) == 0 ? true : false);
    }
    else {
        res = (::strncmp(exp, arg, (equ - arg)) == 0 ? true : false);
    }
    return res;
}

auto arg_val(const std::string& argument) -> std::string
{
    const char* arg = argument.c_str();
    const char* equ = ::strchr(arg, '=');
    const char* val = "";
    if(equ != nullptr) {
        val = equ + 1;
    }
    return val;
}

}

auto Program::init(const ArgList& args) -> bool
{
    static const std::locale new_locale("");
    static const std::locale old_locale(std::locale::global(new_locale));

    auto do_init = [&]() -> bool
    {
        int argi = -1;
        for(auto& arg : args) {
            if(++argi == 0) {
                continue;
            }
            else if(arg == "-h") {
                return false;
            }
            else if(arg == "--help") {
                return false;
            }
            else if(detail::arg_is(arg, "--bank0")) {
                Globals::bank0 = detail::arg_val(arg);
            }
            else if(detail::arg_is(arg, "--bank1")) {
                Globals::bank1 = detail::arg_val(arg);
            }
            else if(detail::arg_is(arg, "--bank2")) {
                Globals::bank2 = detail::arg_val(arg);
            }
            else if(detail::arg_is(arg, "--bank3")) {
                Globals::bank3 = detail::arg_val(arg);
            }
            else {
                throw std::runtime_error(std::string("invalid argument") + ' ' + '\'' + arg + '\'');
            }
        }
        return true;
    };

    return do_init();
}

auto Program::main(const ArgList& args) -> void
{
    std::unique_ptr<VirtualMachine> vm;

    auto do_main = [&]() -> void
    {
        vm = std::make_unique<VirtualMachine>();

        return vm->main();
    };

    return do_main();
}

auto Program::help(const ArgList& args) -> void
{
    auto do_help = [&](std::ostream& stream) -> void
    {
        stream << "Usage: " << detail::program_name(args) << " [OPTIONS...]"         << std::endl;
        stream << ""                                                                 << std::endl;
        stream << "Options:"                                                         << std::endl;
        stream << ""                                                                 << std::endl;
        stream << "  -h, --help                    display this help and exit"       << std::endl;
        stream << ""                                                                 << std::endl;
        stream << "  --bank0={filename}            specifies the ram bank #0 (16kB)" << std::endl;
        stream << "  --bank1={filename}            specifies the ram bank #1 (16kB)" << std::endl;
        stream << "  --bank2={filename}            specifies the ram bank #2 (16kB)" << std::endl;
        stream << "  --bank3={filename}            specifies the ram bank #3 (16kB)" << std::endl;
        stream << ""                                                                 << std::endl;
        stream << "Values:"                                                          << std::endl;
        stream << ""                                                                 << std::endl;
        stream << "  Bank #0: " << Globals::bank0                                    << std::endl;
        stream << "  Bank #1: " << Globals::bank1                                    << std::endl;
        stream << "  Bank #2: " << Globals::bank2                                    << std::endl;
        stream << "  Bank #3: " << Globals::bank3                                    << std::endl;
        stream << ""                                                                 << std::endl;
    };

    return do_help(std::cout);
}

}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

using namespace core;

int main(int argc, char* argv[])
{
    const ArgList args(argv, argv + argc);

    try {
        if(Program::init(args) != false) {
            Program::main(args);
        }
        else {
            Program::help(args);
        }
    }
    catch(const std::exception& e) {
        Console::error(e.what());
        return EXIT_FAILURE;
    }
    catch(...) {
        Console::error("error!");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
