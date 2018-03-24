#pragma once
#include "physics/line.h"
#include "core/curve.h"
#include <bgfx/bgfx.h>
#include <stdlib.h>

// player = { id, character, score, input }

namespace Content {

typedef enum {GROUNDED, JUMPING, SWINGING} CharacterState;
struct Character {
    float position[3];
    
    LineSegment* current_segment;
    Curve movement;

    CharacterState state;

    bgfx::VertexBufferHandle vbh;
    bgfx::IndexBufferHandle ibh;
    bgfx::ProgramHandle program;

  public:
    void init(const char* path);
    void draw();
    void sample(float t);
    void update(float dt, const uint8_t* keyboard_state);
    void reset();
};
}
