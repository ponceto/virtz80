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
#include "globals.h"
#include "program.h"
#include "emulator.h"

// ---------------------------------------------------------------------------
// some useful utilities
// ---------------------------------------------------------------------------

namespace {

auto program_name(const core::ArgList& args) -> const char*
{
    const char* arg = args[0].c_str();
    const char* sep = ::strrchr(arg, '/');
    if(sep != nullptr) {
        arg = (sep + 1);
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
        val = (equ + 1);
    }
    return val;
}

auto yes_or_no(const bool boolean) -> std::string
{
    if(boolean != false) {
        return "yes";
    }
    return "no";
}

}

// ---------------------------------------------------------------------------
// core::Program
// ---------------------------------------------------------------------------

namespace core {

auto Program::init(const ArgList& args) -> bool
{
    static const std::locale new_locale("");
    static const std::locale old_locale(std::locale::global(new_locale));

    auto do_parse = [&]() -> bool
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
            else if(arg == "--zexall") {
                Globals::bank0 = "assets/zexall.rom";
            }
            else if(arg == "--basic") {
                Globals::bank0 = "assets/basic.rom";
            }
            else if(arg == "--turbo") {
                Globals::turbo = true;
            }
            else if(arg_is(arg, "--bank0")) {
                Globals::bank0 = arg_val(arg);
            }
            else if(arg_is(arg, "--bank1")) {
                Globals::bank1 = arg_val(arg);
            }
            else if(arg_is(arg, "--bank2")) {
                Globals::bank2 = arg_val(arg);
            }
            else if(arg_is(arg, "--bank3")) {
                Globals::bank3 = arg_val(arg);
            }
            else {
                throw std::runtime_error(std::string("invalid argument") + ' ' + '\'' + arg + '\'');
            }
        }
        return true;
    };

    auto do_init = [&]() -> bool
    {
        Globals::init();

        return do_parse();
    };

    return do_init();
}

auto Program::main(const ArgList& args) -> void
{
#ifdef __EMSCRIPTEN__
    auto em_main_loop = +[](Emulator* emulator) -> void
    {
        if(emulator->running()) {
            emulator->loop();
        }
        else {
            emulator = (delete emulator, nullptr);
            ::emscripten_cancel_main_loop();
        }
    };
#endif

    auto main_loop = [&]() -> void
    {
        std::unique_ptr<Emulator> emulator(new Emulator());
#ifdef __EMSCRIPTEN__
        ::emscripten_set_main_loop_arg(reinterpret_cast<em_arg_callback_func>(em_main_loop), emulator.release(), 0, 1);
#else
        while(emulator->running()) {
            emulator->loop();
        }
#endif
    };

    auto do_main = [&](std::ostream& stream) -> void
    {
        stream << "Z80 Virtual Machine"                               << std::endl;
        stream << ""                                                  << std::endl;
        stream << "  - turbo" << " ... " << yes_or_no(Globals::turbo) << std::endl;
        stream << "  - bank0" << " ... " << Globals::bank0            << std::endl;
        stream << "  - bank1" << " ... " << Globals::bank1            << std::endl;
        stream << "  - bank2" << " ... " << Globals::bank2            << std::endl;
        stream << "  - bank3" << " ... " << Globals::bank3            << std::endl;
        stream << ""                                                  << std::endl;

        return main_loop();
    };

    return do_main(std::cout);
}

auto Program::help(const ArgList& args) -> void
{
    auto do_help = [&](std::ostream& stream) -> void
    {
        stream << "Usage: " << program_name(args) << " [OPTIONS...]"                   << std::endl;
        stream << ""                                                                   << std::endl;
        stream << "Options:"                                                           << std::endl;
        stream << ""                                                                   << std::endl;
        stream << "  -h, --help                    display this help and exit"         << std::endl;
        stream << ""                                                                   << std::endl;
        stream << "  --zexall                      run the Zexall test suite"          << std::endl;
        stream << "  --basic                       run the Microsoft BASIC"            << std::endl;
        stream << "  --turbo                       run the emulation at maximum speed" << std::endl;
        stream << ""                                                                   << std::endl;
        stream << "  --bank0={filename}            specifies the ram bank #0 (16kB)"   << std::endl;
        stream << "  --bank1={filename}            specifies the ram bank #1 (16kB)"   << std::endl;
        stream << "  --bank2={filename}            specifies the ram bank #2 (16kB)"   << std::endl;
        stream << "  --bank3={filename}            specifies the ram bank #3 (16kB)"   << std::endl;
        stream << ""                                                                   << std::endl;
    };

    return do_help(std::cout);
}

}

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

using namespace base;
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
