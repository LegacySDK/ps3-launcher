#include "utils.h"

#include <stdlib.h>
#include <string.h>

extern unsigned char msx[];
u32 font_offset;
u32 font_w;
u32 font_h;
u32 font_s;

void InitFont(void){
    u32 *texture_mem;
    u32 *texture_pointer;

    texture_mem = tiny3d_AllocTexture(1024 * 1024);

    if (!texture_mem) return;

    texture_pointer = texture_mem;
    for (int n = 0; n < 254; n++) {
        for (int m = 0; m < 8; m++) {
            for (int o = 0; o < 8; o++) {
                int p = o +
                        (n & 15) * 8 +
                        m * 128 +
                        (n / 16) * 1024;

                if ((msx[(n * 8) + m] << o) & 128)
                    texture_pointer[p] = 0xffffffff;
                else
                    texture_pointer[p] = 0;
            }
        }
    }

    font_offset = tiny3d_TextureOffset(texture_pointer);

    font_w = 128;
    font_h = 128;
    font_s = 128 * 4;
}

void PrintChar(float x, float y, float z, u8 ch, u32 color) {
    float u, v, u2, v2;

    u = ((float)(8 * (ch & 15))) / 128.0f;
    u2 = u + 7.99f / 128.0f;

    v = ((float)(8 * (ch >> 4))) / 128.0f;
    v2 = v + 7.99f / 128.0f;

    tiny3d_SetTexture(
        0,
        font_offset,
        font_w,
        font_h,
        font_s,
        TINY3D_TEX_FORMAT_A8R8G8B8,
        1
    );

    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, z);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(u, v);

    tiny3d_VertexPos(x + 16.0f, y, z);
    tiny3d_VertexTexture(u2, v);

    tiny3d_VertexPos(x + 16.0f, y + 16.0f, z);
    tiny3d_VertexTexture(u2, v2);

    tiny3d_VertexPos(x, y + 16.0f, z);
    tiny3d_VertexTexture(u, v2);

    tiny3d_End();
}

void PrintStr(float x, float y, float z, char *ch, u32 color) {
    while (*ch) {
        PrintChar(x, y, z, (u8)*ch, color);

        x += 16.0f;
        ch++;
    }
}

void DrawRect(float x, float y, float z, float w, float h, u32 rgba)
{
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, z);
    tiny3d_VertexColor(rgba);

    tiny3d_VertexPos(x + w, y, z);
    tiny3d_VertexPos(x + w, y + h, z);
    tiny3d_VertexPos(x, y + h, z);

    tiny3d_End();
}

void DrawCorners2d(float x, float y, float z, u32 rgba)
{
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, z);
    tiny3d_VertexColor(rgba);

    tiny3d_VertexPos(x + 32.0f, y, z);
    tiny3d_VertexPos(x + 32.0f, y + 32.0f, z);
    tiny3d_VertexPos(x, y + 32.0f, z);

    tiny3d_End();
}