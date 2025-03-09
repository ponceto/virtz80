/*
 * sio-core.h - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifndef __DEV_SIO_CORE_H__
#define __DEV_SIO_CORE_H__

// ---------------------------------------------------------------------------
// forward declarations
// ---------------------------------------------------------------------------

namespace sio {

class Instance;
class Interface;

}

// ---------------------------------------------------------------------------
// sio::State
// ---------------------------------------------------------------------------

namespace sio {

struct State
{
    int     rx      = -1; /* input file descriptor  */
    int     tx      = -1; /* output file descriptor */
    uint8_t status  =  0; /* status register        */
    uint8_t control =  0; /* control register       */
    uint8_t rx_data =  0; /* receive data register  */
    uint8_t tx_data =  0; /* transmit data register */
    uint8_t enabled =  0; /* is enabled             */
};

}

// ---------------------------------------------------------------------------
// sio::Instance
// ---------------------------------------------------------------------------

namespace sio {

class Instance
{
public: // public interface
    Instance(Interface&, int rx, int tx);

    Instance(const Instance&) = delete;

    Instance& operator=(const Instance&) = delete;

    virtual ~Instance() = default;

    auto reset() -> void;

    auto clock() -> void;

    auto rd_stat(uint8_t data) -> uint8_t;

    auto wr_ctrl(uint8_t data) -> uint8_t;

    auto rd_data(uint8_t data) -> uint8_t;

    auto wr_data(uint8_t data) -> uint8_t;

    auto print(uint8_t data) -> uint8_t;

    auto operator->() -> State*
    {
        return &_state;
    }

protected: // protected data
    Interface& _interface;
    State      _state;
};

}

// ---------------------------------------------------------------------------
// sio::Interface
// ---------------------------------------------------------------------------

namespace sio {

class Interface
{
public: // public interface
    Interface() = default;

    Interface(const Interface&) = default;

    Interface& operator=(const Interface&) = default;

    virtual ~Interface() = default;

    virtual auto sio_intr_rq(Instance&) -> void = 0;
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __DEV_SIO_CORE_H__ */
