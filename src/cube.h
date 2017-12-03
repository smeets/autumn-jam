#include <bgfx/bgfx.h>

struct CubeData {
    bgfx::VertexBufferHandle vbh;
    bgfx::IndexBufferHandle ibh;
    bgfx::ProgramHandle program;
};

struct PosColorVertex {
    float x, y, z;
    uint32_t abgr; /* reverse? */

    static void init() {
        decl.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    }

    static bgfx::VertexDecl decl;
};

void init_cube(CubeData& data);
void draw_cube(CubeData& data, const uint8_t* keyboard_state);