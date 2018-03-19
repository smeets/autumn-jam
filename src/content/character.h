#pragma once
#include <bgfx/bgfx.h>
#include <stdlib.h>

// player = { id, character, score, input }

namespace Content {

typedef enum {GROUNDED, JUMPING, SWINGING} CharacterState;
struct Character {
    float position[3];
    float velocity[3];
    float acceleration[3];
    CharacterState state;

    bgfx::VertexBufferHandle vbh;
    bgfx::IndexBufferHandle ibh;
    bgfx::ProgramHandle program;

  public:
    void init(const char* path);
    void draw();
    void update(float dt, const uint8_t* keyboard_state);
    void reset();
};
}
