#include <iostream>
#include "ExternalProcess.h"

struct _offsets
{
    DWORD localPlayer = 0x0524B63C;
    DWORD fJump = 0x524CEB4;
    DWORD flags = 0x104;
} offsets;

int main()
{
    ExternalProcess csgo(L"csgo.exe");
    uintptr_t localPlayer = NULL;
    uintptr_t clientDLLBase = csgo.GetModuleBaseAddress(L"client.dll");
    
    while (localPlayer == NULL)
    {
        localPlayer = csgo.ReadMemory<uintptr_t>(clientDLLBase + offsets.localPlayer);
    }

    std::cout << std::hex << "LocalPlayer: " << localPlayer << "\nModuleBase: " << clientDLLBase << "\n";

    while (true)
    {
        BYTE flag = csgo.ReadMemory<BYTE>(localPlayer + offsets.flags);

        if (GetAsyncKeyState(VK_SPACE) && flag == (1 << 0))
        {
            std::cout << "jump" << "\n";
            csgo.WriteMemory<DWORD>(clientDLLBase + offsets.fJump, 6);
        }

        Sleep(1);
    }
}