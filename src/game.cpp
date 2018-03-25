#include <SDL2/SDL.h>
#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <stdio.h>
#include <stdlib.h>

#include "content/character.h"
#include "core/camera.h"
#include "game.h"
#include "types.h"
#include "physics/line.h"

void State::enter() {
    Vertex box[4] = { Vertex{-4,0}, Vertex{4,0},
                      Vertex{4, -4}, Vertex{-4, -4} };

    Vertex box2[4] = { Vertex{5, 10}, Vertex{6, 10},
                       Vertex{6, 0}, Vertex{5, 0} };

    for (usize i = 0; i < 4; ++i) {
        segments[i].setup(box[i], box[((i+1)%4)]);
        segments[i].next = &segments[(i+1)%4];

        segments[4+i].setup(box2[i], box2[(i+1)%4]);
        segments[4+i].next = &segments[4+(i+1)%4];
    }

    segments[8].setup(-100,-100,-100,-100);
    segments[9].setup(-100,-100,-100,-100);

    players[0].init("/content/character/");
    players[0].current_segment = &segments[0]; 
}
static f32 time = 0;
State* State::update(float dt) {
    const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
    time += dt; //0.008f;
    
    players[0].update(time, keyboard_state);
    players[0].collision(time, segments);

    players[0].draw();
    return this;
}

void State::exit() {}

/* Settings::screen */
Game::Game() : current_state(nullptr), camera(10, 800, 600, 0.01f, 1000.0f) {}

void Game::init() {
    current_state = new State();
    current_state->enter();
}

bool Game::update(float dt) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return true;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                uint32_t width = uint32_t(event.window.data1);
                uint32_t height = uint32_t(event.window.data2);
                bgfx::reset(width, height, BGFX_RESET_VSYNC);
                camera.resize(float(width), float(height));
            }
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
           if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q]) return true;
     printf("key event: %s\n", SDL_GetKeyName(event.key.keysym.sym));
            break;
        default:
            break;
        }
    }

    bgfx::setViewRect(0, 0, 0, uint16_t(camera.screen_width),
                      uint16_t(camera.screen_height));
    bgfx::touch(0);

    float view[16];
    float at[3] = {0.0f, 0.0f, 0.0f};
    bx::mtxLookAt(view, camera.eye, at);
    bgfx::setViewTransform(0, view, camera.proj);

    State* next = current_state->update(dt);
    if (next != current_state) {
        current_state->exit();
        current_state = next;
        current_state->enter();
    }

    return false;
}

void Game::reset() {
    current_state->exit();
    delete current_state;
}
