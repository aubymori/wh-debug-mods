// ==WindhawkMod==
// @id              debug-window-creation
// @name            [DEBUG] Window Creation
// @description     Determine process and module that created a window of a certain class
// @version         1.0.0
// @author          aubymori
// @github          https://github.com/aubymori
// @include         *
// ==/WindhawkMod==

LPCWSTR s_szClass = L"CiceroUIWndFrame";

#define TextualClassName(x) (((ULONG_PTR)x & ~(ULONG_PTR)0xffff) != 0)

using CreateWindowExW_t = decltype(&CreateWindowExW);
CreateWindowExW_t CreateWindowExW_orig;
HWND WINAPI CreateWindowExW_hook(
    DWORD     dwExStyle,
    LPCWSTR   lpClassName,
    LPCWSTR   lpWindowName,
    DWORD     dwStyle,
    int       X,
    int       Y,
    int       nWidth,
    int       nHeight,
    HWND      hWndParent,
    HMENU     hMenu,
    HINSTANCE hInstance,
    LPVOID    lpParam
)
{
    HWND hRes = CreateWindowExW_orig(
        dwExStyle, lpClassName, lpWindowName,
        dwStyle, X, Y, nWidth, nHeight, hWndParent,
        hMenu, hInstance, lpParam
    );

    if (lpClassName != NULL
    && TextualClassName(lpClassName))
    {
        if (0 == wcscmp(s_szClass, lpClassName)
        )
        {
            Wh_Log(L"created: 0x%X", hRes);
            WCHAR szModuleName[256] = { 0 };
            Wh_Log(L"GetModuleFileNameW: 0x%X", GetModuleFileNameW(
                hInstance,
                szModuleName,
                ARRAYSIZE(szModuleName)
            ));
            Wh_Log(L"Module responsible for creating window: \"%s\"", szModuleName);
            ZeroMemory(szModuleName, sizeof(szModuleName));
            Wh_Log(L"GetModuleFileNameW: 0x%X", GetModuleFileNameW(
                GetModuleHandleW(NULL),
                szModuleName,
                ARRAYSIZE(szModuleName)
            ));
            Wh_Log(L"Current process: \"%s\"", szModuleName);
        }
    }

    return hRes;
}

BOOL Wh_ModInit(void)
{
    Wh_SetFunctionHook(
        (void *)CreateWindowExW,
        (void *)CreateWindowExW_hook,
        (void **)&CreateWindowExW_orig
    );
    return TRUE;
}