#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <stdio.h>


BOOL getProcess(const WCHAR* procName, HANDLE &hProc);


int main()
{
	HANDLE hProc;

	BOOL status = getProcess( L"Spotify.exe", hProc );

	return 0;
}


BOOL getProcess(const WCHAR* procName, HANDLE &hProc)
{
	HANDLE hProcSnap;
	PROCESSENTRY32 peProc;

	// Create snapshot of all running processes
	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hProcSnap == INVALID_HANDLE_VALUE)
	{
		std::cout << "ERROR: INVALID_HANDLE_VALUE while taking process snapshot";
		return FALSE;
	}

	// Needed by documentation
	peProc.dwSize = sizeof(PROCESSENTRY32);

	// Loop through all processes
	while (Process32Next(hProcSnap, &peProc))
	{
		// Find wanted process - we need to use wcscmp insted of strcmp 
		// because we work with WCHAR instead of CHAR
		if (wcscmp(peProc.szExeFile, procName) == 0)
		{
			// Return handle to wanted process in the parameter
			hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, peProc.th32ProcessID);
			return TRUE;
		}
	}

	// std:wcout need to be used because we work with WCHAR
	std::wcout << "ERROR: Process " << procName << " not found\n";
	CloseHandle(hProcSnap);
	
	return FALSE;
}