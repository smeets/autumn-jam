#include "cube.h"
#include "hello.h"
#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/file.h>
#include <bx/math.h>
#include <bx/readerwriter.h>
#include <stdint.h>
#include <stdio.h>
#include <string>

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

void draw_cube(CubeData& data) {
    time = time + 0.001f;
    float xx = 1;
    float yy = 1;

    float mtx[16];
    // bx::mtxRotateXY(mtx, time + xx * 0.21f, time + yy * 0.37f);
    bx::mtxScale(mtx, (sin(time) + 1) * 2, (sin(time) + 1) * 2, 1);
    // mtx[12] = -15.0f + float(xx) * 3.0f;
    // mtx[13] = -15.0f + float(yy) * 3.0f;
    // mtx[14] = 0.0f;

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