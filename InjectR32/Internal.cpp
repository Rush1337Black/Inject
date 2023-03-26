#pragma once

#include "library.h"

class Internal
{
	public:

		LPVOID GetNTOpenFile()
		{
			return GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");
		}


		bool CheckFileExist(const char* name)
		{
			if (FILE* file = fopen(name, "r"))
			{
				fclose(file);
				return true;
			}
			return false;
		}

		bool ExecuteBypass(HANDLE hProcess)
		{
			if (GetNTOpenFile())
			{
				char originalBytes[5];
				memcpy(originalBytes, GetNTOpenFile(), 5);
				if (WriteProcessMemory(hProcess, GetNTOpenFile(), originalBytes, 5, NULL))
				{
					std::cout << "VAC Bypassed.." << std::endl;
					return TRUE;
				}
			}
			return FALSE;
		}

		bool Backup(HANDLE hProcess)
		{
			if (GetNTOpenFile())
			{
				char Orig[5];
				memcpy(Orig, GetNTOpenFile(), 5);
				WriteProcessMemory(hProcess, GetNTOpenFile(), Orig, 0, 0);
				return TRUE;
			}
			return FALSE;
		}
};