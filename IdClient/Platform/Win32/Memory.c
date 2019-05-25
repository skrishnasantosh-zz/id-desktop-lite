#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdlib.h>

void* idAllocateMemory(const size_t destSize)
{
	HANDLE hHeap = GetProcessHeap();
	if (hHeap == NULL)
		return NULL;

	void* mem = HeapAlloc(hHeap, HEAP_ZERO_MEMORY, destSize);

	return mem;
}

void* idReallocateMemory(void* block, const size_t destSize)
{
	HANDLE hHeap = GetProcessHeap();
	if (hHeap == NULL)
		return NULL;

	void* mem = HeapReAlloc(hHeap, HEAP_ZERO_MEMORY, block, destSize);

	return mem;
}

int idFreeMemory(void** block)
{
	if (block != NULL)
	{
		HANDLE hHeap = GetProcessHeap();
		if (hHeap == NULL)
			return NULL;

		int ret = HeapFree(hHeap, 0, *block);
		*block = NULL;

		return ret;
	}
}
