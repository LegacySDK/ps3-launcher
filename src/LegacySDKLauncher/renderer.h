#pragma once
#include <stdint.h>

class renderer {
public:
	static void clear(uint32_t width, uint32_t height, uint32_t bufferOffset, uint32_t color);
};