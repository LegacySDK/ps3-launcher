#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/process.h>
#include <sysutil/sysutil.h>
#include <tiny3d.h>
#include <io/pad.h>

#include "utils.h"

SYS_PROCESS_PARAM(1001, 0x10000);

static void sysutil_exit_callback(u64 status, u64 param, void *userdata) {
    if (status == SYSUTIL_EXIT_GAME)
    {
        exit(0);
    }
}

int main(int argc, char *argv[]) {
    padInfo padinfo;
    padData paddata;

    sysUtilRegisterCallback(0, sysutil_exit_callback, NULL);
    ioPadInit(7);
    tiny3d_Init(1024 * 1024);
    InitFont();

    const char *text = "Hello from LegacySDK!";

    while (1) {
        sysUtilCheckCallback();
        ioPadGetInfo(&padinfo);

        for (int i = 0; i < MAX_PADS; i++) {
            if (padinfo.status[i]) {
                ioPadGetData(i, &paddata);

                if (paddata.BTN_START) {
                    return 0;
                }
            }
        }

        tiny3d_Clear(0x808080FF, TINY3D_CLEAR_ALL);
        tiny3d_Project2D();

        /*
            tiny3d_AlphaTest(1, 0x10, TINY3D_ALPHA_FUNC_GEQUAL);
            tiny3d_BlendFunc(
                1,
                TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA,
                TINY3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA | TINY3D_BLEND_FUNC_DST_ALPHA_ZERO,
                TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD
            );
        */

        float text_width = strlen(text) * 16.0f;
        float x = (848.0f - text_width) / 2.0f;
        float y = (512.0f - 16.0f) / 2.0f;

        PrintStr(x, y, 0, (char *)text, 0xffffffff);
        tiny3d_Flip();
    }
    return 0;
}