#include "screen.h"

#include <cell/gcm.h>
#include <cell/resc.h>
#include <cell/sysmodule.h>

#include <stdlib.h>
#include <string.h>

static void* gCommandBuffer		= NULL;
static void* gHostMemory		= NULL;

unsigned int screen::gWidth		= 1280;
unsigned int screen::gHeight	= 720;

// some bullshit
uint32_t screen::gColorBufferOffsets[2] = { 0, 0 };
uint32_t screen::gCurrentBuffer = 0;

bool screen::gInitialized = false;

#define COMMAND_SIZE		(65536)			// 64KB
#define BUFFER_SIZE			(1024 * 1024)	// 1MB

void screen::displayRes(uint32_t& outWidth, uint32_t& outHeight, uint8_t& outResId) {
	CellVideoOutState videoState;
	cellVideoOutGetState(CELL_VIDEO_OUT_PRIMARY, 0, &videoState);

	CellVideoOutResolution resolution;
	cellVideoOutGetResolution(videoState.displayMode.resolutionId, &resolution);

	outWidth = resolution.width;
	outHeight = resolution.height;
	outResId = videoState.displayMode.resolutionId;
}

bool screen::init(uint32_t width, uint32_t height) {
	cellSysmoduleLoadModule(CELL_SYSMODULE_GCM_SYS);

	CellGcmConfig config;
	cellGcmGetConfiguration(&config); // not sure but this should just work
	// yes it will, https://research.ncl.ac.uk/game/mastersdegree/workshops/ps3introductiontogcm/tutorial6.pdf page 12	
	gHostMemory = memalign(1024 * 1024, BUFFER_SIZE);
	if (cellGcmInit(COMMAND_SIZE, BUFFER_SIZE, gHostMemory) != CELL_OK) {
		std::cout << "cellGcmInit failed !" << std::endl;
		return false;
	}
	
	uint8_t resolutionId = CELL_VIDEO_OUT_RESOLUTION_720;
	if (width == 0 || height == 0) {
		displayRes(gWidth, gHeight, resolutionId);
	}
	else {
		gWidth = width;
		gHeight = height;
	}

	std::cout << "initializing display/screen at " << gWidth << "x" << gHeight << std::endl;

	CellVideoOutConfiguration videoConfig;
	memset(&videoConfig, 0, sizeof(CellVideoOutConfiguration));
	videoConfig.resolutionId = resolutionId;
	videoConfig.format = CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_X8R8G8B8;
	videoConfig.aspect = CELL_VIDEO_OUT_ASPECT_16_9;

	if (cellVideoOutConfigure(CELL_VIDEO_OUT_PRIMARY, &videoConfig, NULL, 0) != CELL_OK) {
		std::cout << "Failed to configure video output!" << std::endl;
		return false;
	}

	cellGcmSetWaitFlip(gCellGcmCurrentContext);

	uint8_t* lcMemBase = static_cast<uint8_t*>(config.localAddress);
	uint32_t pitch = gWidth * 4;
	uint32_t bufferSize = pitch * gHeight;

	bufferSize = (bufferSize + 0xFFFF) & ~0xFFFF;

	for (int i = 0; i < 2; ++i) {
		void* bfradr = lcMemBase + (i * bufferSize);

		if (cellGcmAddressToOffset(bfradr, &gColorBufferOffsets[i]) != CELL_OK) {
			std::cerr << " failed to alloc local mem for bfr " << i << "!" << std::endl;
			return false;
		}
		if (cellGcmSetDisplayBuffer(i, gColorBufferOffsets[i], pitch, gWidth, gHeight) != CELL_OK) {
			std::cerr << " failed to set display bfr " << i << "!" << std::endl;
			return false;
		}
	}

	gInitialized = true;
	return true;
}

void screen::flip() {
	if (!gInitialized) return;

	cellGcmSetFlip(gCellGcmCurrentContext, gCurrentBuffer);
	cellGcmFlush(gCellGcmCurrentContext);
	cellGcmSetWaitFlip(gCellGcmCurrentContext);
	gCurrentBuffer = !gCurrentBuffer;
}

void screen::shutdown() {
	if (!gInitialized) return;
	cellGcmFinish(gCellGcmCurrentContext, 0);

	if (gHostMemory) {
		free(gHostMemory);
		gHostMemory = NULL;
	}

	cellSysmoduleUnloadModule(CELL_SYSMODULE_GCM_SYS);
	gInitialized = false;
}