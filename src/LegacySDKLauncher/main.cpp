#include <sys/process.h>
#include <unistd.h>
#include "screen.h"
#include "renderer.h"
#include "helper.h"
/*
 *  Have fun reading whatever the fuck this is.
 * ---------------------------------------------
 *  Made by: gh:miskkaaa and gh:hlelo120
 *  Join our motherfucking discord server:
 *  https://discord.gg/s7ynJawdPq
*/

SYS_PROCESS_PARAM(1001, 0x10000);

int main()
{
	if (!screen::init()) {
		return -1;
	}

	// i wanna put return 0; here but i dont
	// want it to instantly crash

	bool running = true;
	while (running) {
		renderer::clear(
			screen::GetWidth(),
			screen::GetHeight(),
			screen::GetCurrentBufferOffset(),
			helper::clearcol
		);

		screen::flip();
	}

	screen::shutdown();
	return 0;
	// fixed!!
}