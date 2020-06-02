#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <stdio.h>
#include <bx/allocator.h>
#include <bgfx/platform.h>
#include <bx/math.h>

bgfx::VertexBufferHandle m_vbh;
bgfx::IndexBufferHandle m_ibh;

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

struct CubeComponent {

    float pos_x, pos_y, pos_z;
    float rot_x, rot_y, rot_z;
    bgfx::IndexBufferHandle ibh;
    bgfx::ProgramHandle m_program;

public:

    void Init(float x, float y, float z, float rx, float ry, float rz) {
        pos_x = x;
        pos_y = y;
        pos_z = z;
        rot_x = rx;
        rot_y = ry;
        rot_z = rz;
        bgfx::ShaderHandle vsh = loadShader("../Sne/src/Sne/Ressources/vs_cubes.bin");
        bgfx::ShaderHandle fsh = loadShader("../Sne/src/Sne/Ressources/fs_cubes.bin");
        m_program = bgfx::createProgram(vsh, fsh, true);

        PosColorVertex::init();
        // Create static vertex buffer.
        m_vbh = bgfx::createVertexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(cubeVertices, sizeof(cubeVertices))
            , PosColorVertex::ms_layout
        );

        // Create static index buffer for triangle list rendering.
        m_ibh = bgfx::createIndexBuffer(
            // Static data can be passed with bgfx::makeRef
            bgfx::makeRef(cubeTriList, sizeof(cubeTriList))
        );

        ibh = m_ibh;
    }

    void render() {
        unsigned int counter = 0;
        const bx::Vec3 at = { 0.0f, 0.0f,  0.0f };
        const bx::Vec3 eye = { 0.0f, 0.0f, -30.0f };
        float view[16];
        bx::mtxLookAt(view, eye, at);
        float proj[16];
        bx::mtxProj(proj, 60.0f, float(1024) / float(720), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
        bgfx::setViewTransform(0, view, proj);

        float mtx[16];
        bx::mtxRotateXYZ(mtx, rot_x, rot_y, rot_z);
        bgfx::setTransform(mtx);

        // Set view 0 default viewport.
        bgfx::setViewRect(0, 0, 0, uint16_t(1024), uint16_t(720));
        mtx[12] = pos_x;
        mtx[13] = pos_y;
        mtx[14] = pos_z;

        // Set model matrix for rendering.
        bgfx::setTransform(mtx);

        // Set vertex and index buffer.
        bgfx::setVertexBuffer(0, m_vbh);
        bgfx::setIndexBuffer(ibh);

        // Submit primitive for rendering to view 0.
        bgfx::submit(0, m_program);

        // Advance to next frame. Rendering thread will be kicked to
        // process submitted rendering primitives.
    }
};
