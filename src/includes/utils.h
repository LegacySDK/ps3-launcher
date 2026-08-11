#ifndef UTILS_H
#define UTILS_H

#include <tiny3d.h>
#include <ppu-types.h>

extern u32 font_offset;
extern u32 font_w;
extern u32 font_h;
extern u32 font_s;

void InitFont(void);
void DrawCorners2d(float x, float y, float z, u32 rgba);
void DrawRect(float x, float y, float z, float w, float h, u32 rgba);
void PrintChar(float x, float y, float z, u8 ch, u32 color);
void PrintStr(float x, float y, float z, char *ch, u32 color);

#endif