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

#include "video.h"
#include "config.h"
#include "common/host.h"
#include <SDL2/SDL.h>

static bool initialized = false;
// SDL window handle.
static SDL_Window* window = nullptr;
// SDL rendering context.
static SDL_Renderer* renderer = nullptr;

static void createWindow();
static void destroyWindow();
static void createRenderer();
static void destroyRenderer();
static void grabMouse();

void Video::init() {
    if (initialized) {
        return;
    }
    if (SDL_Init(SDL_INIT_VIDEO)) {
        PANIC("failed to initialize video system: %s", SDL_GetError());
    }
    createWindow();
    createRenderer();
    initialized = true;
}

void Video::shutdown() {
    if (!initialized) {
        return;
    }
    destroyWindow();
    destroyRenderer();
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    initialized = false;
}

void Video::refresh() {
    SDL_RenderPresent(renderer);
}

static void createWindow() {
    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;
    int w = 0;
    int h = 0;
    u32 flags = SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN_DESKTOP;
    window = SDL_CreateWindow(PACKAGE_STRING, x, y, w, h, flags);
    if (!window) {
        PANIC("failed to create window: %s", SDL_GetError());
    }
    SDL_SetWindowMinimumSize(window, 320, 240);
    grabMouse();
}

static void destroyWindow() {
    SDL_DestroyWindow(window);
    window = nullptr;
}

static void createRenderer() {
    int index = -1;
    u32 flags = SDL_RENDERER_TARGETTEXTURE;
    renderer = SDL_CreateRenderer(window, index, flags);
    if (!renderer) {
        PANIC("failed to create video renderer: %s", SDL_GetError());
    }
    // Blank out the full screen area in case there is any junk in
    // the borders that won't otherwise be overwritten.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

static void destroyRenderer() {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
}

static void grabMouse() {
    // Relative mode for continuous mouse motion.
    SDL_SetRelativeMouseMode(SDL_TRUE);
    // Do not show cursor.
    SDL_ShowCursor(SDL_FALSE);
    // Zero mouse accumulation, so the camera doesn't go flying.
    SDL_GetRelativeMouseState(nullptr, nullptr);
}
