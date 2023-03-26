#pragma once

#include "library.h"

class Process
{
   private:

		PROCESSENTRY32 processentry;
		DWORD ProcessId;

		uintptr_t GetModuleBaseAddress(DWORD pid, const char* modName)
		{
			HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);

			if (hSnap != INVALID_HANDLE_VALUE) 
			{
				MODULEENTRY32 modEntry;
				modEntry.dwSize = sizeof(modEntry);
				if (Module32First(hSnap, &modEntry)) 
				{
					do 
					{
						if (!strcmp(modEntry.szModule, modName)) 
						{
							CloseHandle(hSnap);
							return (uintptr_t)modEntry.modBaseAddr;
						}
					} while (Module32Next(hSnap, &modEntry));
				}
			}
			return NULL;
		}
	public:

      DWORD GetId(const char* ProcessName)
		{
			HANDLE Snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

			processentry.dwSize = sizeof(processentry);

			if (!Process32First(Snapshot, &processentry))
				return NULL;

			do 
			{
				if (!strcmp(processentry.szExeFile, ProcessName))
				{
					CloseHandle(Snapshot);
					return ProcessId = processentry.th32ProcessID;
				}
			} while (Process32Next(Snapshot, &processentry));

			CloseHandle(Snapshot);
			return NULL;
      }

		HANDLE Open()
		{
			if (!ProcessId)
				return NULL;

			return OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
		}

		uintptr_t GetModuleBase(const char* client)
		{

			if (!ProcessId)
				return NULL;

			return GetModuleBaseAddress(ProcessId, client);
		}

		bool LoadLibraryInject(const char* Dll)
		{
			if (ProcessId == NULL)
				return false;

			char CustomDLL[MAX_PATH];
			GetFullPathName(Dll, MAX_PATH, CustomDLL, 0);

			HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, ProcessId);
			LPVOID allocatedMem = VirtualAllocEx(hProcess, NULL, sizeof(CustomDLL), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

			if (!WriteProcessMemory(hProcess, allocatedMem, CustomDLL, sizeof(CustomDLL), NULL))
				return FALSE;

			CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, allocatedMem, 0, 0);

			if (hProcess)
				CloseHandle(hProcess);

			return TRUE;
		}
};