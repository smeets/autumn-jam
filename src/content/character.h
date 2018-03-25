#pragma once
#include "physics/line.h"
#include "physics/jump.h"
#include "core/curve.h"
#include <bgfx/bgfx.h>
#include <stdlib.h>

// player = { id, character, score, input }

namespace Content {

typedef enum {GROUNDED, JUMPING, SWINGING} CharacterState;
struct Character {
    float position[3];
    
    LineSegment* current_segment;
    JumpSegment* jump_segment;

    LinearCurve movement; // reset by switching line segment
    TimeCurve   jump;     // reset by jumping

    CharacterState state;

    bgfx::VertexBufferHandle vbh;
    bgfx::IndexBufferHandle ibh;
    bgfx::ProgramHandle program;

  public:
    void init(const char* path);
    void draw();
    void sample(float t);
    void update(float dt, const uint8_t* keyboard_state);
    void collision(float t, LineSegment segments[10]);
    void reset();
};
}
