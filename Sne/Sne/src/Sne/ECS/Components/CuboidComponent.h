#pragma once
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <stdio.h>
#include <bx/allocator.h>
#include <bgfx/platform.h>
#include <bx/math.h>

struct PosColorVertex
{
    float x;
    float y;
    float z;
    uint32_t abgr;

    static void init()
    {
        ms_layout
            .begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
            .end();
    };

    static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex cubeVertices[] =
{
    {-1.0f,  1.0f,  1.0f, 0xff000000 },
    { 1.0f,  1.0f,  1.0f, 0xff0000ff },
    {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
    { 1.0f, -1.0f,  1.0f, 0xff00ffff },
    {-1.0f,  1.0f, -1.0f, 0xffff0000 },
    { 1.0f,  1.0f, -1.0f, 0xffff00ff },
    {-1.0f, -1.0f, -1.0f, 0xffffff00 },
    { 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t cubeTriList[] =
{
    0, 1, 2,
    1, 3, 2,
    4, 6, 5,
    5, 6, 7,
    0, 2, 4,
    4, 2, 6,
    1, 5, 3,
    5, 7, 3,
    0, 4, 1,
    4, 5, 1,
    2, 3, 6,
    6, 3, 7,
};

bgfx::ShaderHandle loadShader(const char* FILENAME)
{
    FILE* file = fopen(FILENAME, "rb");
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    const bgfx::Memory* mem = bgfx::alloc(fileSize + 1);
    fread(mem->data, 1, fileSize, file);
    mem->data[mem->size - 1] = '\0';
    fclose(file);

    return bgfx::createShader(mem);
}

struct CuboidComponent {
    bgfx::VertexBufferHandle vbh;
    bgfx::IndexBufferHandle ibh;
    bgfx::ProgramHandle m_program;

public:
    void Init()
    {
        bgfx::ShaderHandle vsh = loadShader("../Sne/src/Sne/Resources/vs_cubes.bin");
        bgfx::ShaderHandle fsh = loadShader("../Sne/src/Sne/Resources/fs_cubes.bin");
        m_program = bgfx::createProgram(vsh, fsh, true);

        PosColorVertex::init();
        // Create static vertex buffer.
        vbh = bgfx::createVertexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(cubeVertices, sizeof(cubeVertices))
            , PosColorVertex::ms_layout
        );

        // Create static index buffer for triangle list rendering.
        ibh = bgfx::createIndexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(cubeTriList, sizeof(cubeTriList))
        );
    }
};
