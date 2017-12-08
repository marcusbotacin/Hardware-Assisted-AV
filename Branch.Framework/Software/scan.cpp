// Hardware Assisted AV
// Software component
// Marcus Botacin - 2017
// Federal University of Paraná (UFPR)

#include<Windows.h>	// System header
#include"scan.h"	// Project

// Open handler to scanned process
HANDLE open_proc(DWORD pid)
{
	return OpenProcess(PROCESS_VM_READ,FALSE,pid);
}

// get module
HMODULE get_module(HANDLE hProc)
{
	// what code portion you want to scan
	return CODE_PORTION;
}

// dump process memory to check byte pattern
DWORD read_mem(HANDLE hProc)
{
	SIZE_T bytes;
	DWORD buffer;

	// memory region
	UINT64 base = (UINT64)get_module(hProc);

	// dump
	ReadProcessMemory(hProc,&base,&buffer,sizeof(buffer),&bytes);

	// if read
	if(bytes)
		return buffer;

	// not read
	return 0;
}

// Scan procedure
int scan(DWORD pid, psignature db)
{
	DWORD mem;

	// open target process
	HANDLE hProc = open_proc(pid);
	if(!hProc)
	{
		return FAIL;
	}

	// dump memory
	mem = read_mem(hProc);
	if(!mem)
	{
		return FAIL;
	}

	// SCAN 
    // using db
    // removed for testing purposes
    // return index -- according db

	// If here, clean
	CloseHandle(hProc);
	return CLEAN;
}
