/*
 * application.h - Copyright (c) 2001-2025 - Olivier Poncet
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
#ifndef __BASE_Application_h__
#define __BASE_Application_h__

// ---------------------------------------------------------------------------
// base::Application
// ---------------------------------------------------------------------------

namespace base {

class Application
{
public: // public interface
    Application(const std::string& name);

    Application(const Application&) = delete;

    Application& operator=(const Application&) = delete;

    virtual ~Application() = default;

    virtual auto loop() -> void = 0;

    virtual auto quit() -> void = 0;

    auto running() const -> bool
    {
        return _quit == false;
    }

    auto stopped() const -> bool
    {
        return _quit != false;
    }

protected: // protected data
    const std::string _name;
    bool              _quit;
};

}

// ---------------------------------------------------------------------------
// End-Of-File
// ---------------------------------------------------------------------------

#endif /* __BASE_Application_h__ */
