#pragma once

#include <SDL.h>

class Window {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface* screen_buffer;
    SDL_Surface* argb_buffer;
    SDL_Texture* texture;

public:
    Window();
    Window(Window& other) = delete;
    ~Window();
    Window& operator=(const Window& other) = delete;
};
