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
#include "common/memory.h"
#include "client/client.h"
#include "server/server.h"
#include <SDL2/SDL.h>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>

// Tracks whether the application has entered error mode.
static bool error_entered = false;

void Host::run(int argc, char** argv) {
    // Init subsystems.
    Host::init(argc, argv);
    // Run game loop.
    Host::runLoop();
}

void Host::quit() {
    // If we are in error mode, avoid shutting
    // down subsystems, as this could trigger
    // a recursive error.
    if (!error_entered) {
        Host::shutdown();
    }
    std::exit(EXIT_SUCCESS);
}

void Host::error(const char* fmt, ...) {
    static char error[1024];
    va_list ap;

    if (error_entered) {
        std::fprintf(stderr, "ERROR: recursive error after: %s\n", error);
        std::exit(EXIT_FAILURE);
    }
    error_entered = true;

    va_start(ap, fmt);
    std::vsnprintf(error, sizeof(error), fmt, ap);
    va_end(ap);

    // Show user the error message before shutting down.
    std::fprintf(stderr, "%s\n", error);
    Host::shutdown();

    std::exit(EXIT_FAILURE);
}

void Host::init(int argc, char** argv) {
    Host::initTimer();
    Mem::init();
    Host::initEvents();
    Client::init();
    Server::init();
}

void Host::shutdown() {
    Server::shutdown();
    Client::shutdown();
    Host::shutdownEvents();
    Mem::shutdown();
    Host::shutdownTimer();
    SDL_Quit();
}
