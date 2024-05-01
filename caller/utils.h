#pragma once
#include <Windows.h>





void* AllocRWX(size_t size) {
	return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}