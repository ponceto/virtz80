/*
 * sio-core.cc - Copyright (c) 2001-2025 - Olivier Poncet
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
#include <termios.h>
#include <unistd.h>
#include <poll.h>
#include "sio-core.h"

// ---------------------------------------------------------------------------
// sio::aliases
// ---------------------------------------------------------------------------

namespace {

using pollfd_type  = struct pollfd;

}

// ---------------------------------------------------------------------------
// sio::Terminal
// ---------------------------------------------------------------------------

namespace sio {

struct Terminal
{
    using termios_type = struct termios;

    static auto get_attributes(int fd, termios_type& attributes) -> void
    {
        if(::tcgetattr(fd, &attributes) != 0) {
            throw std::runtime_error("tcgetattr() has failed");
        }
    };

    static auto set_attributes(int fd, termios_type& attributes) -> void
    {
        if(::tcsetattr(fd, TCSANOW, &attributes) != 0) {
            throw std::runtime_error("tcsetattr() has failed");
        }
    };

    static auto adjust_iflags(termios_type& attributes) -> void
    {
        attributes.c_iflag &= ~0;
        attributes.c_iflag |=  INLCR;
        attributes.c_iflag &= ~IGNCR;
        attributes.c_iflag &= ~ICRNL;
    };

    static auto adjust_oflags(termios_type& attributes) -> void
    {
        attributes.c_oflag &= ~0;
        attributes.c_oflag |=  OPOST;
        attributes.c_oflag |=  ONOCR;
        attributes.c_oflag |=  OFILL;
        attributes.c_oflag |=  OFDEL;
    };

    static auto adjust_lflags(termios_type& attributes) -> void
    {
        attributes.c_lflag &= ~0;
        attributes.c_lflag &= ~ECHO;
        attributes.c_lflag |=  ECHOE;
        attributes.c_lflag &= ~ICANON;
        attributes.c_lflag |=  ISIG;
    };

    static auto setup_rx(int rx) -> void
    {
        termios_type attributes = {};
        get_attributes(rx, attributes);
        adjust_iflags(attributes);
        adjust_lflags(attributes);
        set_attributes(rx, attributes);
    }

    static auto setup_tx(int tx) -> void
    {
        termios_type attributes = {};
        get_attributes(tx, attributes);
        adjust_oflags(attributes);
        adjust_lflags(attributes);
        set_attributes(tx, attributes);
    }
};

}

// ---------------------------------------------------------------------------
// sio::ACIA
// ---------------------------------------------------------------------------

namespace sio {

struct ACIA
{
    static constexpr uint16_t SR_RDRF = 0b00000001; /* Receive Data Register Full   */
    static constexpr uint16_t SR_TDRE = 0b00000010; /* Transmit Data Register Empty */
    static constexpr uint16_t SR_DCD  = 0b00000100; /* Data Carrier Detect          */
    static constexpr uint16_t SR_CTS  = 0b00001000; /* Clear-To-Send                */
    static constexpr uint16_t SR_FE   = 0b00010000; /* Framing Error                */
    static constexpr uint16_t SR_OVRN = 0b00100000; /* Receiver Overrun             */
    static constexpr uint16_t SR_PE   = 0b01000000; /* Parity Error                 */
    static constexpr uint16_t SR_IRQ  = 0b10000000; /* Interrupt Request            */

    static constexpr uint16_t CR_CR0  = 0b00000001; /* Counter Divide Select        */
    static constexpr uint16_t CR_CR1  = 0b00000010; /* Counter Divide Select        */
    static constexpr uint16_t CR_CR2  = 0b00000100; /* Word Select                  */
    static constexpr uint16_t CR_CR3  = 0b00001000; /* Word Select                  */
    static constexpr uint16_t CR_CR4  = 0b00010000; /* Word Select                  */
    static constexpr uint16_t CR_CR5  = 0b00100000; /* Transmitter Control          */
    static constexpr uint16_t CR_CR6  = 0b01000000; /* Transmitter Control          */
    static constexpr uint16_t CR_IRQ  = 0b10000000; /* Receive Interrupt Enable     */
};

}

// ---------------------------------------------------------------------------
// sio::Instance
// ---------------------------------------------------------------------------

namespace sio {

Instance::Instance(Interface& interface)
    : _interface(interface)
    , _state()
{
}

auto Instance::reset() -> void
{
    if(_state.rx == -1) {
        _state.rx = 0; /* stdin */
        Terminal::setup_rx(_state.rx);
    }
    if(_state.tx == -1) {
        _state.tx = 1; /* stdout */
        Terminal::setup_tx(_state.tx);
    }
    _state.status  = 0;
    _state.control = 0;
    _state.rx_data = 0;
    _state.tx_data = 0;
    _state.enabled = 0;
}

auto Instance::clock() -> void
{
    constexpr int acia_count = 2;
    pollfd_type   pollfds[acia_count];
    pollfd_type&  acia_rd(pollfds[0]);
    pollfd_type&  acia_wr(pollfds[1]);

    auto do_init = [&]() -> void
    {
        for(auto& pollfd : pollfds) {
            pollfd.fd      = -1;
            pollfd.events  =  0;
            pollfd.revents =  0;
        }
        if((_state.rx >= 0) && ((_state.status & ACIA::SR_RDRF) == 0)) {
            acia_rd.fd     = (_state.rx);
            acia_rd.events = (POLLIN | POLLERR | POLLHUP);
        }
        if((_state.tx >= 0) && ((_state.status & ACIA::SR_TDRE) == 0)) {
            acia_wr.fd     = (_state.tx);
            acia_wr.events = (POLLOUT | POLLERR | POLLHUP);
        }
    };

    auto do_poll = [&]() -> void
    {
        const auto ready = ::poll(pollfds, acia_count, 0);
        if(ready > 0) {
            if((acia_rd.revents & POLLIN) != 0) {
                _state.status |= (ACIA::SR_RDRF | ACIA::SR_IRQ);
                const auto rc = ::read(_state.rx, &_state.rx_data, sizeof(_state.rx_data));
                static_cast<void>(rc);
            }
            if((acia_wr.revents & POLLOUT) != 0) {
                _state.status |= (ACIA::SR_TDRE);
                const auto rc = ::write(_state.tx, &_state.tx_data, sizeof(_state.tx_data));
                static_cast<void>(rc);
            }
        }
        if(((_state.status  & ACIA::SR_IRQ) != 0)
        && ((_state.control & ACIA::CR_IRQ) != 0)) {
            _interface.sio_intr_rq(*this);
        }
    };

    auto do_clock = [&]() -> void
    {
        if(_state.enabled != 0) {
            do_init();
            do_poll();
        }
    };

    return do_clock();
}

auto Instance::rd_acia_stat(uint8_t data) -> uint8_t
{
    if(_state.enabled == 0) {
        _state.status  &= ~ACIA::SR_RDRF; /* Receive Data Register Full   */
        _state.status  |=  ACIA::SR_TDRE; /* Transmit Data Register Empty */
        _state.status  &= ~ACIA::SR_DCD;  /* Data Carrier Detect          */
        _state.status  &= ~ACIA::SR_CTS;  /* Clear-To-Send                */
        _state.status  &= ~ACIA::SR_FE;   /* Framing Error                */
        _state.status  &= ~ACIA::SR_OVRN; /* Receiver Overrun             */
        _state.status  &= ~ACIA::SR_PE;   /* Parity Error                 */
        _state.status  &= ~ACIA::SR_IRQ;  /* Interrupt Request            */
        _state.enabled |=  1;
    }
    return data = _state.status;
}

auto Instance::wr_acia_ctrl(uint8_t data) -> uint8_t
{
    if(_state.enabled == 0) {
        _state.status  &= ~ACIA::SR_RDRF; /* Receive Data Register Full   */
        _state.status  |=  ACIA::SR_TDRE; /* Transmit Data Register Empty */
        _state.status  &= ~ACIA::SR_DCD;  /* Data Carrier Detect          */
        _state.status  &= ~ACIA::SR_CTS;  /* Clear-To-Send                */
        _state.status  &= ~ACIA::SR_FE;   /* Framing Error                */
        _state.status  &= ~ACIA::SR_OVRN; /* Receiver Overrun             */
        _state.status  &= ~ACIA::SR_PE;   /* Parity Error                 */
        _state.status  &= ~ACIA::SR_IRQ;  /* Interrupt Request            */
        _state.enabled |=  1;
    }
    return _state.control = data;
}

auto Instance::rd_acia_data(uint8_t data) -> uint8_t
{
    if((_state.status & ACIA::SR_RDRF) != 0) {
        _state.status &= ~(ACIA::SR_RDRF | ACIA::SR_IRQ);
        data = _state.rx_data;
        if(data == 0x7f) {
            data = '\b';
        }
    }
    return data;
}

auto Instance::wr_acia_data(uint8_t data) -> uint8_t
{
#ifdef __EMSCRIPTEN__
    if(data == '\r') {
        return data;
    }
#endif
    if((_state.status & ACIA::SR_TDRE) != 0) {
        _state.status &= ~(ACIA::SR_TDRE);
        _state.tx_data = data;
    }
    return data;
}

auto Instance::print(uint8_t data) -> uint8_t
{
    _state.tx_data = data;
    if(_state.tx >= 0) {
        const auto rc = ::write(_state.tx, &data, sizeof(data));
        static_cast<void>(rc);
    }
    return data;
}

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------
