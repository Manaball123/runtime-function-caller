#pragma once
#include <Windows.h>





inline void* AllocRWX(size_t size) {
	return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}

//qword movabs only, in little endian
inline short ArgIdxToRegister(int idx) {
	switch (idx) {
	case 0:
		return 0xb948;
	case 1:
		return 0xba48;
	case 2:
		return 0xb849;
	case 3:
		return 0xb949;
	}
}