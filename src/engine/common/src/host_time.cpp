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
static u32 start_time = 0;

void Host::initTimer() {
    if (initialized) {
        return;
    }
    if (SDL_InitSubSystem(SDL_INIT_TIMER)) {
        PANIC("failed to initialize timer: %s", SDL_GetError());
    }
    start_time = SDL_GetTicks();
    initialized = true;
}

void Host::shutdownTimer() {
    if (initialized) {
        SDL_QuitSubSystem(SDL_INIT_TIMER);
        initialized = false;
    }
}

u32 Host::getTime() {
    return SDL_GetTicks() - start_time;
}

void Host::sleep(u32 ms) {
    SDL_Delay(ms);
}
