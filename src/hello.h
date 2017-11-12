/*
 * Copyright 2011-2017 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#ifndef BGFX_UTILS_H_HEADER_GUARD
#define BGFX_UTILS_H_HEADER_GUARD

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>
#include <bx/pixelformat.h>
#include <bx/readerwriter.h>

///
bgfx::ShaderHandle loadShader(bx::FileReaderI* _reader, const char* _name);

///
bgfx::ProgramHandle loadProgram(bx::FileReaderI* _reader, const char* _vsName,
                                const char* _fsName);

#endif // BGFX_UTILS_H_HEADER_GUARD