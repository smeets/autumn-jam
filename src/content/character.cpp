#include "character.h"
#include "graphics/pcv.h"
#include "hello.h"
#include <SDL2/SDL.h>
#include <bx/file.h>

static Graphics::PosColorVertex cubeVertices[] = {
    {-1.0f, 1.0f, 1.0f, 0xff000000},   {1.0f, 1.0f, 1.0f, 0xff0000ff},
    {-1.0f, -1.0f, 1.0f, 0xff00ff00},  {1.0f, -1.0f, 1.0f, 0xff00ffff},
    {-1.0f, 1.0f, -1.0f, 0xffff0000},  {1.0f, 1.0f, -1.0f, 0xffff00ff},
    {-1.0f, -1.0f, -1.0f, 0xffffff00}, {1.0f, -1.0f, -1.0f, 0xffffffff},
};

static uint16_t cubeTriStrip[] = {
    0, 1, 2, 3, 7, 1, 5, 0, 4, 2, 6, 7, 4, 5,
};

using namespace Content;

void Character::init(const char* path) {
    for (size_t i = 0; i < 3; i++) {
        position[i] = 0.0f;
        velocity[i] = 0.0f;
    }
    grounded = false;

    Graphics::PosColorVertex::init();

    vbh = bgfx::createVertexBuffer(
        bgfx::makeRef(cubeVertices, sizeof(cubeVertices)),
        Graphics::PosColorVertex::decl);

    ibh = bgfx::createIndexBuffer(
        bgfx::makeRef(cubeTriStrip, sizeof(cubeTriStrip)));

    bx::FileReader reader;
    program = loadProgram(&reader, "vs_cube", "fs_cube");
}

void Character::draw() {
    float mtx[16];
    bx::mtxIdentity(mtx);
    for (size_t i = 0; i < 3; i++)
        mtx[12 + i] = position[i];

    bgfx::setTransform(mtx);
    bgfx::setVertexBuffer(0, vbh);
    bgfx::setIndexBuffer(ibh);
    bgfx::setState(0 | BGFX_STATE_DEFAULT | BGFX_STATE_PT_TRISTRIP);
    bgfx::submit(0, program);
}

static float SPEED = 400;
static float GRAVITY = -16;
static float JUMP = 18;
void Character::update(float dt, const uint8_t* keyboard_state) {
    float input[2] = {0, 0};

    if (keyboard_state[SDL_SCANCODE_RIGHT])
        input[0] += 1;

    if (keyboard_state[SDL_SCANCODE_LEFT])
        input[0] -= 1;

    if (keyboard_state[SDL_SCANCODE_UP] && grounded) {
        velocity[1] = JUMP;
        grounded = false;
    }

    velocity[0] += input[0] * SPEED * dt;
    velocity[0] *= 0.8f;

    if (!grounded)
        velocity[1] += GRAVITY * dt;

    position[0] += velocity[0] * dt;
    position[1] += velocity[1] * dt;

    if (position[1] <= -1) {
        position[1] = -1;
        velocity[1] = 0;
        grounded = true;
    }
}

void Character::reset() {
    for (size_t i = 0; i < 3; i++) {
        position[i] = 0.0f;
        velocity[i] = 0.0f;
    }
}
