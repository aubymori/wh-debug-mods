// ==WindhawkMod==
// @id              debug-classic-theme
// @name            [DEBUG] Classic Theme
// @description     Force classic theme for a certain process
// @version         1.0.0
// @author          aubymori
// @github          https://github.com/aubymori
// @include         explorer.exe
// @compilerOptions -luxtheme
// ==/WindhawkMod==

#include <uxtheme.h>

BOOL Wh_ModInit(void)
{
    SetThemeAppProperties(0);
    return TRUE;
}