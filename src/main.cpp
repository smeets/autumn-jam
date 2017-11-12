#include <SDL2/SDL.h>
#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <stdio.h>
#include <stdlib.h>
#undef main

#include "cube.h"

#include <bx/os.h>

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

    bgfx::reset(width, height, BGFX_RESET_NONE);
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

    CubeData data;
    init_cube(data);

    bool exit = false;
    SDL_Event event;
    printf("running\n");
    float at[3] = {0.0f, 0.0f, 0.0f};
    float eye[3] = {0.0f, 0.0f, -35.0f};
    while (!exit) {
        bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
        bgfx::touch(0);
        float view[16];
        bx::mtxLookAt(view, eye, at);

        float proj[16];
        // bx::mtxOrtho
        bx::mtxProj(proj, 60.0f, float(width) / float(height), 0.1f, 100.0f,
                    bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);

        draw_cube(data);

        // Set view 0 default viewport.
        bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));

        bgfx::dbgTextClear();
        const bgfx::Stats* stats = bgfx::getStats();
        bgfx::dbgTextPrintf(0, 0, 0x0f,
                            "Window %dW x %dH px, backbuffer %dW x "
                            "%dH px, debug text %dW x %dH in "
                            "characters.",
                            width, height, stats->width, stats->height,
                            stats->textWidth, stats->textHeight);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                exit = true;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    width = uint32_t(event.window.data1);
                    height = uint32_t(event.window.data2);
                    bgfx::reset(width, height, BGFX_RESET_NONE);
                }
                break;
            default:
                break;
            }
        }

        bgfx::frame();
    }

    printf("exit\n");

    bgfx::shutdown();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
