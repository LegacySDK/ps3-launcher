#include <stdio.h>
#include <sys/process.h>
#include <stdlib.h>
#include <string.h>
#include <sysutil/sysutil.h>
#include <tiny3d.h>
#include <io/pad.h>

SYS_PROCESS_PARAM(1001, 0x10000);

static void sysutil_exit_callback(u64 status, u64 param, void *userdata) {
    if (status == SYSUTIL_EXIT_GAME) {
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    sysUtilRegisterCallback(0, sysutil_exit_callback, NULL);
    ioPadInit(7);
    tiny3d_Init(1024 * 1024);

    padInfo padinfo;
    padData paddata;

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
        tiny3d_Flip();
    }

    return 0;
}