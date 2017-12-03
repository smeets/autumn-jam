#include "cube.h"
#include "hello.h"
#include <SDL2/SDL_keycode.h>
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/file.h>
#include <bx/math.h>
#include <stdio.h>

bgfx::VertexDecl PosColorVertex::decl;
static PosColorVertex cubeVertices[] = {
    {-1.0f, 1.0f, 1.0f, 0xff000000},   {1.0f, 1.0f, 1.0f, 0xff0000ff},
    {-1.0f, -1.0f, 1.0f, 0xff00ff00},  {1.0f, -1.0f, 1.0f, 0xff00ffff},
    {-1.0f, 1.0f, -1.0f, 0xffff0000},  {1.0f, 1.0f, -1.0f, 0xffff00ff},
    {-1.0f, -1.0f, -1.0f, 0xffffff00}, {1.0f, -1.0f, -1.0f, 0xffffffff},
};

static uint16_t cubeTriStrip[] = {
    0, 1, 2, 3, 7, 1, 5, 0, 4, 2, 6, 7, 4, 5,
};

void init_cube(CubeData& data) {
    printf("init_cube\n");

    PosColorVertex::init();

    printf("create vbh\n");
    // Create static vertex buffer.
    data.vbh = bgfx::createVertexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(cubeVertices, sizeof(cubeVertices)),
        PosColorVertex::decl);

    printf("create ibh\n");
    // Create static index buffer.
    data.ibh = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(cubeTriStrip, sizeof(cubeTriStrip)));

    printf("loadProgram\n");
    bx::FileReader reader;
    data.program = loadProgram(&reader, "vs_cube", "fs_cube");
}
static float time = 0;
static float pos[3] = {0, 0, 0};
static float vel[3] = {0, 0, 0};
static float speed = 400;
static float jump_impulse = 18;
static float gravity = -16;
static float timestep = 0.016f;
static bool grounded = false;

void draw_cube(CubeData& data, const uint8_t* keyboard_state) {
    time = time + timestep;
    float xx = 1;
    float yy = 1;
    float input[2] = {0, 0};

    if (keyboard_state[SDL_SCANCODE_RIGHT])
        input[0] += 1;

    if (keyboard_state[SDL_SCANCODE_LEFT])
        input[0] -= 1;

    if (keyboard_state[SDL_SCANCODE_UP] && grounded) {
        vel[1] = jump_impulse;
        grounded = false;
    }

    vel[0] += input[0] * speed * timestep;
    vel[0] *= 0.80f;

    if (!grounded)
        vel[1] += gravity * timestep;

    for (size_t i = 0; i < 2; i++)
        pos[i] += vel[i] * timestep;

    if (pos[1] <= -1) {
        pos[1] = -1;
        vel[1] = 0;
        grounded = true;
    }

    float mtx[16];
    bx::mtxIdentity(mtx);
    mtx[12] = pos[0];
    mtx[13] = pos[1];
    mtx[14] = pos[2];

    // Set model matrix for rendering.
    bgfx::setTransform(mtx);

    // Set vertex and index buffer.
    bgfx::setVertexBuffer(0, data.vbh);
    bgfx::setIndexBuffer(data.ibh);

    // Set render states.
    bgfx::setState(0 | BGFX_STATE_DEFAULT | BGFX_STATE_PT_TRISTRIP);

    // Submit primitive for rendering to view 0.
    bgfx::submit(0, data.program);
}