#include "window.h"
#include "config.h"
#include <format>

using std::domain_error;

#define WINDOW_WIDTH  (320)
#define WINDOW_HEIGHT (240)

#define BUFFER_WIDTH  WINDOW_WIDTH
#define BUFFER_HEIGHT (200)

static constexpr Uint32 pixel_format{SDL_PIXELFORMAT_ARGB8888};


static SDL_Window* createWindow() {
    const auto title{PACKAGE_STRING};
    constexpr int x{SDL_WINDOWPOS_CENTERED};
    constexpr int y{SDL_WINDOWPOS_CENTERED};
    constexpr int w{};
    constexpr int h{};
    constexpr Uint32 flags{
        SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN_DESKTOP
    };
    const auto window{SDL_CreateWindow(title, x, y, w, h, flags)};
    if (!window) {
        const auto error{
            std::format("Error creating window: {}", SDL_GetError())
        };
        throw domain_error{error};
    }
    SDL_SetWindowMinimumSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);
    return window;
}

static SDL_Renderer* createRenderer(SDL_Window* window) {
    if (!window) {
        return nullptr;
    }

    constexpr int index{-1};
    constexpr Uint32 flags{SDL_RENDERER_TARGETTEXTURE};
    const auto renderer{SDL_CreateRenderer(window, index, flags)};

    // Important: Set the "logical size" of the rendering context. At the same
    // time this also defines the aspect ratio that is preserved while scaling
    // and stretching the texture into the window.
    SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);

    // Blank out the full screen area in case there is any junk in
    // the borders that won't otherwise be overwritten.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    return renderer;
}

static SDL_Surface* createScreenBuffer() {
    constexpr Uint32 flags{};
    constexpr int w{BUFFER_WIDTH};
    constexpr int h{BUFFER_HEIGHT};
    constexpr int depth{8};
    constexpr Uint32 r{};
    constexpr Uint32 g{};
    constexpr Uint32 b{};
    constexpr Uint32 a{};

    const auto screen_buffer{
        SDL_CreateRGBSurface(flags, w, h, depth, r, g, b, a)
    };
    SDL_FillRect(screen_buffer, nullptr, 0);

    return screen_buffer;
}

static SDL_Surface* createArgbBuffer() {
    constexpr Uint32 flags{};
    constexpr int w{BUFFER_WIDTH};
    constexpr int h{BUFFER_HEIGHT};
    Uint32 r{};
    Uint32 g{};
    Uint32 b{};
    Uint32 a{};
    int bpp{};

    SDL_PixelFormatEnumToMasks(pixel_format, &bpp, &r, &g, &b, &a);
    const auto argb_buffer{SDL_CreateRGBSurface(flags, w, h, bpp, r, g, b, a)};
    SDL_FillRect(argb_buffer, nullptr, 0);

    return argb_buffer;
}

static SDL_Texture* createTexture(SDL_Renderer* renderer) {
    if (!renderer) {
        return nullptr;
    }
    constexpr int access{SDL_TEXTUREACCESS_STREAMING};
    constexpr int w{BUFFER_WIDTH};
    constexpr int h{BUFFER_HEIGHT};
    const auto texture{SDL_CreateTexture(renderer, pixel_format, access, w, h)};
    return texture;
}

Window::Window()
    : window{createWindow()}
    , renderer{createRenderer(window)}
    , screen_buffer{createScreenBuffer()}
    , argb_buffer{createArgbBuffer()}
    , texture{createTexture(renderer)} {
}

Window::~Window() {
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(argb_buffer);
    SDL_FreeSurface(screen_buffer);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    texture = nullptr;
    argb_buffer = nullptr;
    screen_buffer = nullptr;
    renderer = nullptr;
    window = nullptr;
}
