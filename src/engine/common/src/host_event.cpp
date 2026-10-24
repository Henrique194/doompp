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

#include "common/host.h"
#include <SDL2/SDL.h>

static bool initialized = false;

// Maximum number of events that can be queued.
static constexpr u32 MAX_EVENTS = 256;
// Circular buffer for queued events.
static Event events[MAX_EVENTS];
// Index of the next event to read.
static u32 head = 0;
// Index of the next position to write.
static u32 tail = 0;

static Event getEvent();
static void pollEvents();

void Host::initEvents() {
    if (initialized) {
        return;
    }
    if (SDL_InitSubSystem(SDL_INIT_EVENTS)) {
        PANIC("failed to initialize events system: %s", SDL_GetError());
    }
    initialized = true;
}

void Host::shutdownEvents() {
    if (initialized) {
        SDL_QuitSubSystem(SDL_INIT_EVENTS);
        initialized = false;
    }
}

void Host::eventLoop() {
    while (auto ev = getEvent()) {
        switch (ev.type) {
            case EventType::Quit:
                Host::quit();
                break;
            default:
                break;
        }
    }
}

static Event getEvent() {
    if (head == tail) {
        pollEvents();
    }
    if (head != tail) {
        Event ev = events[tail++];
        tail = tail % MAX_EVENTS;
        return ev;
    }
    u32 ms = Host::getTime();
    return Event::noneEv(ms);
}

static void pollEvents() {
    SDL_Event ev;
    while (SDL_PollEvent(&ev)) {
        switch (ev.type) {
            case SDL_QUIT: {
                u32 ms = ev.quit.timestamp;
                events[head++] = Event::quitEv(ms);
                break;
            }
            default:
                continue;
        }
        head = head % MAX_EVENTS;
    }
}
