/*
 * Copyright (C) Henrique Barateli, <henriquejb194@gmail.com>, et al.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#pragma once

#include "types.h"

// Macro used to terminate the application when an error occurs.
#define PANIC(fmt, ...)                                                        \
    Host::error("ERROR [%s]: " fmt, __func__ __VA_OPT__(, ) __VA_ARGS__)

class Host {
  public:
    [[noreturn]] static void run(int argc, char** argv);
    [[noreturn]] static void quit();
    [[noreturn]] static void error(const char* fmt, ...);

    static u32 getTime();
    static void sleep(u32 ms);

  private:
    static void init(int argc, char** argv);
    static void shutdown();

    static void initTimer();
    static void shutdownTimer();

    static void initEvents();
    static void shutdownEvents();
    static void eventLoop();

    [[noreturn]] static void runLoop();
    static void runFrame(u32 ms);
};

enum class EventType : u32 {
    None,
    Quit,
};

struct EventQuit {
    EventType type;
    u32 time;
};

union Event {
    struct {
        EventType type;
        u32 time;
    };
    EventQuit quit;

    static Event noneEv(u32 ms) {
        return {EventType::None, ms};
    }

    static Event quitEv(u32 ms) {
        return {.quit = {EventType::Quit, ms}};
    }

    operator bool() const {
        return type != EventType::None;
    }
};
