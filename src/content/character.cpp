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
    for (size_t i = 0; i < 3; i++) {
        position[i] = 0.0f;
        velocity[i] = 0.0f;
    }
    state = JUMPING;

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
static float SPEED_MAX = 10;
static float GRAVITY = -16;
static float JUMP = 18;
void Character::update(float dt, const uint8_t* keyboard_state) {
    int input[2] = {0, 0};

    if (keyboard_state[SDL_SCANCODE_RIGHT]) {
        input[0] = 1;
    } else if (keyboard_state[SDL_SCANCODE_LEFT]) {
        input[0] = -1;
    } else {
	input[0] = 0;
    }

    if (keyboard_state[SDL_SCANCODE_UP] && state == GROUNDED) {
        velocity[1] = JUMP;
        state = JUMPING;
    }

    if (state == SWINGING) {
        // accelerating like an ideal pendulum
    }

    if(state == GROUNDED || state == JUMPING) {
	if (input[0] != 0) {
	    acceleration[0] = input[0] * SPEED; // input force
	} else {
	    acceleration[0] = 0;
	}
    }

    if (state == JUMPING) {
        acceleration[1] = GRAVITY;
    }

    velocity[0] += acceleration[0] * dt;
    velocity[0] += sign(velocity[0])*GRAVITY * dt; // friction
    velocity[1] += acceleration[1] * dt;
    velocity[0] = clamp(velocity[0], SPEED_MAX);

    position[0] += velocity[0] * dt;
    position[1] += velocity[1] * dt;


    bool become_grounded = position[1] <= -1;

    if (become_grounded) {
        position[1] = -1;
        velocity[1] = 0;
        acceleration[1] = 0;
    }

    if (become_grounded) {
        state = GROUNDED;
    }
}

void Character::reset() {
    for (size_t i = 0; i < 3; i++) {
        position[i] = 0.0f;
        velocity[i] = 0.0f;
    }
}
