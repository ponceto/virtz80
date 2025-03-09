/*
 * vdu-core.h - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifndef __DEV_VDU_CORE_H__
#define __DEV_VDU_CORE_H__

// ---------------------------------------------------------------------------
// forward declarations
// ---------------------------------------------------------------------------

namespace vdu {

class Instance;
class Interface;

}

// ---------------------------------------------------------------------------
// vdu::State
// ---------------------------------------------------------------------------

namespace vdu {

struct State
{
    uint32_t clock = 4134375;
    uint32_t hfreq = 15750;
    uint32_t vfreq = 60;
    uint32_t hcntr = 0;
    uint32_t vcntr = 0;
};

}

// ---------------------------------------------------------------------------
// vdu::Instance
// ---------------------------------------------------------------------------

namespace vdu {

class Instance
{
public: // public interface
    Instance(Interface&);

    Instance(const Instance&) = delete;

    Instance& operator=(const Instance&) = delete;

    virtual ~Instance() = default;

    auto reset() -> void;

    auto clock() -> void;

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
// vdu::Interface
// ---------------------------------------------------------------------------

namespace vdu {

class Interface
{
public: // public interface
    Interface() = default;

    Interface(const Interface&) = default;

    Interface& operator=(const Interface&) = default;

    virtual ~Interface() = default;

    virtual auto vdu_sync_hs(Instance&, bool hsync) -> void = 0;

    virtual auto vdu_sync_vs(Instance&, bool vsync) -> void = 0;
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __DEV_VDU_CORE_H__ */
