#pragma once

#include "stdafx.h"

class Mem
{
	DWORD GetProcId(const wchar_t* procName);
	uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* moduleName);
	uintptr_t FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);
};

