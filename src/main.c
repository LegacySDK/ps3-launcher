#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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

    const char *title = "LegacySDK PS3 Launcher";
    const char *menuItems[] = {"LAUNCH", "SETTINGS"};
    const int itemCount = sizeof(menuItems) / sizeof(menuItems[0]);
    const float screenW = 848.0f;
    const float screenH = 512.0f;
    const float buttonW = 360.0f;
    const float buttonH = 56.0f;
    const float buttonX = (screenW - buttonW) / 2.0f;
    const float buttonY = 220.0f;
    const float itemSpacing = 84.0f;
    int selectedItem = 0;
    float selectedAnim = 0.0f;
    float arrowPhase = 0.0f;
    int wasUpPressed = 0;
    int wasDownPressed = 0;
    int wasActionPressed = 0;

    while (1) {
        sysUtilCheckCallback();
        ioPadGetInfo(&padinfo);

        int padActive = 0;
        int upPressed = 0;
        int downPressed = 0;
        int actionPressed = 0;

        for (int i = 0; i < MAX_PADS; i++) {
            if (!padinfo.status[i]) continue;
            padActive = 1;
            ioPadGetData(i, &paddata);

            if (paddata.BTN_UP) upPressed = 1;
            if (paddata.BTN_DOWN) downPressed = 1;
            if (paddata.BTN_CROSS || paddata.BTN_CIRCLE || paddata.BTN_START) actionPressed = 1;
        }

        if (upPressed && !wasUpPressed && selectedItem > 0) {
            selectedItem--;
        }
        if (downPressed && !wasDownPressed && selectedItem < itemCount - 1) {
            selectedItem++;
        }

        if (actionPressed && !wasActionPressed) {
            if (selectedItem == 0) {
                tiny3d_Clear(0x808080FF, TINY3D_CLEAR_ALL);
                tiny3d_Project2D();
                tiny3d_AlphaTest(1, 0x80, TINY3D_ALPHA_FUNC_GREATER);
                tiny3d_BlendFunc(
                    1,
                    TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA,
                    TINY3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA | TINY3D_BLEND_FUNC_DST_ALPHA_ZERO,
                    TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD
                );
                tiny3d_Flip();
                return 0;
            }
        }

        wasUpPressed = upPressed;
        wasDownPressed = downPressed;
        wasActionPressed = actionPressed;

        selectedAnim += (selectedItem - selectedAnim) * 0.3f;
        if (selectedAnim < 0.0f) selectedAnim = 0.0f;
        if (selectedAnim > itemCount - 1) selectedAnim = itemCount - 1;

        arrowPhase += 0.1f;
        if (arrowPhase > 6.2831853f) arrowPhase -= 6.2831853f;
        float arrowShift = sinf(arrowPhase) * 5.0f;

        tiny3d_Clear(0x808080FF, TINY3D_CLEAR_ALL);
        tiny3d_Project2D();
        tiny3d_AlphaTest(1, 0x80, TINY3D_ALPHA_FUNC_GREATER);
        tiny3d_BlendFunc(
            1,
            TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA,
            TINY3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA | TINY3D_BLEND_FUNC_DST_ALPHA_ZERO,
            TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD
        );

        float headingWidth = strlen(title) * 16.0f;
        float headingX = (screenW - headingWidth) / 2.0f;
        float headingY = 100.0f;
        DrawRect(headingX - 16.0f, headingY - 12.0f, 0, headingWidth + 32.0f, 32.0f, 0x202020FF);
        PrintStr(headingX, headingY, 0, (char *)title, 0xffffffff);

        for (int i = 0; i < itemCount; i++) {
            float y = buttonY + i * itemSpacing;
            u32 border = (i == selectedItem) ? 0xffffffff : 0x9090b0ff;
            u32 inner = (i == selectedItem) ? 0x404040ff : 0x303550ff;
            u32 textCol = (i == selectedItem) ? 0xffffffff : 0xffd0d0ff;

            DrawRect(buttonX - 4.0f, y - 4.0f, 0, buttonW + 8.0f, buttonH + 8.0f, border);
            DrawRect(buttonX, y, 0, buttonW, buttonH, inner);

            char itemText[64];
            sprintf(itemText, "%s", menuItems[i]);
            float itemWidth = strlen(itemText) * 16.0f;
            float itemX = buttonX + (buttonW - itemWidth) / 2.0f;
            float itemY = y + (buttonH - 16.0f) / 2.0f;

            PrintStr(itemX, itemY, 0, itemText, textCol);
        }

        tiny3d_Flip();
    }
    return 0;
}
