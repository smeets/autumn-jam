#include "game.h"
#include <SDL2/SDL.h>
#include <bgfx/bgfx.h>
#include <bx/allocator.h>
#include <bx/bx.h>
#include <stdio.h>
#include <stdlib.h>
#include <vg/vg.h>
#undef main

BX_PRAGMA_DIAGNOSTIC_PUSH()
BX_PRAGMA_DIAGNOSTIC_IGNORED_CLANG("-Wextern-c-compat")
#include <SDL2/SDL_syswm.h>
BX_PRAGMA_DIAGNOSTIC_POP()

#include <bgfx/platform.h>
#if defined(None) // X11 defines this...
#undef None
#endif // defined(None)

void error(const char* msg) {
    puts(msg);
    exit(EXIT_FAILURE);
}

SDL_Window* init_window(uint32_t width, uint32_t height) {
    SDL_Init(0 | SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow(
        "autumn jam", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width,
        height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    return window;
}

void init_platform_data(SDL_Window* window) {
    SDL_SysWMinfo wmi;
    SDL_VERSION(&wmi.version);
    if (!SDL_GetWindowWMInfo(window, &wmi))
        error("Error on SDL_GetWindowWMInfo");

    bgfx::PlatformData pd;
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    pd.ndt = wmi.info.x11.display;
    pd.nwh = (void*)(uintptr_t)wmi.info.x11.window;
#elif BX_PLATFORM_OSX
    pd.ndt = NULL;
    pd.nwh = wmi.info.cocoa.window;
#elif BX_PLATFORM_WINDOWS
    pd.ndt = NULL;
    pd.nwh = wmi.info.win.window;
#elif BX_PLATFORM_STEAMLINK
    pd.ndt = wmi.info.vivante.display;
    pd.nwh = wmi.info.vivante.window;
#endif // BX_PLATFORM_
    pd.context = NULL;
    pd.backBuffer = NULL;
    pd.backBufferDS = NULL;

    bgfx::setPlatformData(pd);
}

void init_bgfx(uint32_t width, uint32_t height) {
    /* NONE = Autoselect graphics adapter */
    bgfx::init(bgfx::RendererType::OpenGL, BGFX_PCI_ID_NONE);

    bgfx::reset(width, height, BGFX_RESET_VSYNC);
    bgfx::setDebug(BGFX_DEBUG_TEXT);

    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f,
                       0);
}

int main(/*int argc, char** argv*/) {
    uint32_t width = 800;
    uint32_t height = 600;

    printf("init\n");

    auto window = init_window(width, height);

    init_platform_data(window);
    init_bgfx(width, height);

    bool exit = false;
    printf("running\n");

    Game game;
    game.init();

    bgfx::setViewMode(0, bgfx::ViewMode::Sequential);
    while (!exit) {

        bgfx::dbgTextClear();
        bgfx::dbgTextPrintf(0, 0, 0x0f, "Controls: Space = Jump, Q = Exit");

        exit = game.update(0.016f);

        bgfx::frame();
    }

    printf("exit\n");

    game.reset();
    bgfx::shutdown();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
