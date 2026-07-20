#include "renderer.h"
#include <cell/gcm.h>
#include <string.h>

void renderer::clear(uint32_t width, uint32_t height, uint32_t bufferOffset, uint32_t color) {
	CellGcmSurface surface;
	memset(&surface, 0, sizeof(CellGcmSurface));

	surface.colorFormat = CELL_GCM_SURFACE_A8R8G8B8;
	surface.colorTarget = CELL_GCM_SURFACE_TARGET_0;

	surface.colorLocation[0] = CELL_GCM_LOCATION_LOCAL;
	surface.colorOffset[0] = bufferOffset;
	surface.colorPitch[0] = width * 4;

	for (int i = 1; i < 4; ++i) {
		surface.colorLocation[i] = CELL_GCM_LOCATION_LOCAL;
		surface.colorOffset[i] = 0;
		surface.colorPitch[i] = 64;
	}

	surface.type = CELL_GCM_SURFACE_PITCH;
	surface.width = width;
	surface.height = height;
	surface.x = 0;
	surface.y = 0;

	cellGcmSetSurface(gCellGcmCurrentContext, &surface);

	cellGcmSetClearColor(gCellGcmCurrentContext, color);

	cellGcmSetClearSurface(
		gCellGcmCurrentContext,
		CELL_GCM_CLEAR_R |
		CELL_GCM_CLEAR_G |
		CELL_GCM_CLEAR_B |
		CELL_GCM_CLEAR_A
		);
}