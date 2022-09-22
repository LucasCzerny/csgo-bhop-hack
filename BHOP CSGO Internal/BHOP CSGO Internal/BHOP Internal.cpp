// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>

struct _offsets
{
    DWORD localPlayer = 0x0524B63C;
    DWORD fJump = 0x524CEB4;
    DWORD flags = 0x104;
} offsets;


void HackThread()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    uintptr_t clientDLLBase = (uintptr_t)GetModuleHandle(L"client.dll");
    uintptr_t localPlayer = NULL;

    while (localPlayer == NULL)
    {
        localPlayer = *(uintptr_t*)(clientDLLBase + offsets.localPlayer);
    }

    while (true)
    {
        BYTE flag = *(BYTE*)(localPlayer + offsets.flags);

        if (GetAsyncKeyState(VK_SPACE) && flag == (1 << 0))
        {
            *(uintptr_t*)(clientDLLBase + offsets.fJump) = 6;
        }
    }

    FreeConsole();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HackThread, NULL, NULL, NULL);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

