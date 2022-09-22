#include "proc.h"

DWORD Mem::GetProcId(const wchar_t* procName)
{
	DWORD procId = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (_wcsicmp(procEntry.szExeFile, procName) == 0)
				{
					procId = procEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hSnap, &procEntry));
		}
	}

	CloseHandle(hSnap);
	return procId;
}

uintptr_t Mem::GetModuleBaseAddress(DWORD procId, const wchar_t* moduleName)
{
	uintptr_t moduleBaseAddress = 0;
	HANDLE hProc = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);

	if (hProc != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(moduleEntry);

		if (Module32First(hProc, &moduleEntry))
		{
			do
			{
				if (_wcsicmp(moduleEntry.szExePath, moduleName) == 0)
				{
					moduleBaseAddress = (uintptr_t)moduleEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hProc, &moduleEntry));
		}
	}

	CloseHandle(hProc);
	return moduleBaseAddress;
}

uintptr_t Mem::FindDMAAddy(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets)
{
	uintptr_t currentAddress = ptr;

	for (unsigned int offset : offsets)
	{
		ReadProcessMemory(hProc, (void*)ptr, &ptr, sizeof(ptr), nullptr);
		currentAddress += offset;
	}

	return currentAddress;
}