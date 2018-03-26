#include "character.h"
#include <stdio.h>
#include "graphics/pcv.h"
#include "physics/line.h"
#include "physics/jump.h"
#include "physics/cohen.h"
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

static f32 VELOCITY = 4.0f; //

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

void Character::collision(float t, LineSegment segments[10])
{
    if (jump_segment == NULL ||
        t-jump.t0 < 0.1f) return;

    // collision check gg
    AABB dude(Vertex{position[0]-1, position[1]-1}, 2, 2);
    for (usize i = 0; i < 10; ++i) {
        LineSegment copy_segment = segments[i];
        bool intersects = physics::intersect_cohen(
            copy_segment, dude);

        if (intersects) {
            delete jump_segment;
            jump_segment = NULL;

            current_segment = &segments[i];
            PosDir& contact_x = copy_segment.x;
            PosDir& contact_y = copy_segment.y;

            f32 dx = (current_segment->x.p - (contact_x.p+contact_x.d/2));
            f32 dy = (current_segment->y.p - (contact_y.p+contact_y.d/2));
            f32 len = sqrt(dx*dx + dy*dy);
            f32 total = sqrt(current_segment->y.d*current_segment->y.d
                + current_segment->x.d*current_segment->x.d);

            movement.t0 = t;
            movement.t1 = t + total/VELOCITY - len/VELOCITY;
            movement.y0 = len/total;
            break;
        }
    }
}

void Character::sample(float t) {
    // TODO: Move branch higher up in call stack
    if (current_segment != NULL) {
        f32 s = movement.sample(t);
        LineSegment* next = current_segment->lerp(&s, (f32*)&position);

        if (next != current_segment) {
            f32 dist = sqrt(next->x.d*next->x.d +
                            next->y.d*next->y.d);
            movement.t0 = t + 0.016f;
            movement.t1 = t + 0.016f + dist / VELOCITY; // DIST/VELOCITY=time
            movement.y0 = 0;
            movement.y1 = 1;
            printf("s=%f,t=%f\n", movement.sample(t), t);
            printf("(%f,%f) -> (%f,%f) %f m (%f sec)\n",
                current_segment->x.p,
                current_segment->y.p,
                next->x.p + next->x.d,
                next->y.p + next->y.d,
                dist, dist / VELOCITY);
            current_segment = next;
        }
    } else if (jump_segment != NULL) {
        jump_segment->lerp(jump.sample(t), (f32*)&position);
    }

}

void Character::update(float t, const uint8_t* keyboard_state) {
    if (keyboard_state[SDL_SCANCODE_SPACE] &&
        jump_segment == NULL) {
        // movement.t1 = t - 0.016f;
        // movement.y1 = movement.y0 + (t - movement.t0) * 1 // 1 = VELOCITY
        current_segment = NULL;

        // TODO: Malloc scrubs
        jump_segment = new JumpSegment(Vertex{position[0], position[1]});
        jump.t0 = t;
    }

    sample(t);
}

void Character::reset() {
    for (size_t i = 0; i < 3; i++) {
        position[i] = 0.0f;
    }
}
