#include "character.h"
#include <stdio.h>
#include "graphics/pcv.h"
#include "physics/line.h"
#include "core/curve.h"
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

double sign(double x) {
    double eps = 1e-16;
    if (x > eps) return 1.0;
    else if (x < -eps) return -1.0;
    return 0.0;
}
double clamp(double x, double limit) {
    if(x > limit) return limit;
    else if (x < -limit) return -limit;
    else return x;
}

void Character::init(const char* path) {
    reset();

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

void Character::sample(float t) {
    f32 s = movement.sample(t);
    LineSegment* next = current_segment->lerp(&s, (f32*)&position);
    if (next != current_segment) {
        movement.t0 = t + 0.016f;
        movement.t1 = t + 0.016f + 1.0f;
        printf("s=%f,t=%f\n", movement.sample(t), t);
        printf("(%f,%f) -> (%f,%f)\n", 
            current_segment->x.p, 
            current_segment->y.p, 
            next->x.p + next->x.d, 
            next->y.p + next->y.d);
        current_segment = next;
    }
}

void Character::update(float t, const uint8_t* keyboard_state) {
    bool jump = false;
    if (keyboard_state[SDL_SCANCODE_UP]) {
        jump = true;
    }
    sample(t);
}

void Character::reset() {
    for (size_t i = 0; i < 3; i++) {
        position[i] = 0.0f;
    }
}
