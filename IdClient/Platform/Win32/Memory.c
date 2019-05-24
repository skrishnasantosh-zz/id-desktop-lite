#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdlib.h>

void* AllocateMemory(const size_t destSize)
{
	void* mem = calloc(1, destSize);

	return mem;
}

void* ReallocateMemory(void* block, const size_t destSize)
{
	void* mem = realloc(block, destSize);

	return mem;
}

void FreeMemory(void** block)
{
	if (block != NULL)
	{
		free(*block);
		*block = NULL;
	}
}