#pragma once

#include <iostream>
#include <stdint.h>

class screen {
public:
	static bool init(uint32_t width = 0, uint32_t = 0);
	static void flip();
	static void shutdown();

	static unsigned int GetWidth()  {  return gWidth;  };
	static unsigned int GetHeight() {  return gHeight; };

	static uint32_t GetCurrentBufferOffset() { return gColorBufferOffsets[gCurrentBuffer]; }

private:
	static unsigned int gWidth;
	static unsigned int gHeight;

	static uint32_t gColorBufferOffsets[2];
	static uint32_t gCurrentBuffer;
	static bool gInitialized;

	static void displayRes(uint32_t& outWidth, uint32_t& outHeight, uint8_t& outResId);
};