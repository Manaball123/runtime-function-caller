#pragma once
#include <Windows.h>



inline void* AllocRWX(size_t size) {
	return VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
}

//qword movabs only, in little endian
inline const std::vector<UCHAR>& ArgIdxToQWord(int idx) {
	const static std::vector<std::vector<UCHAR>> instructions = { 
	{ 0x48, 0xb9 }, //rcx
	{ 0x48, 0xba },	//rdx
	{ 0x49, 0xb8 }, //r8
	{ 0x49, 0xb9 }  //r9
	};
	return instructions[idx];
}

//dword mov
inline const std::vector<UCHAR>& ArgIdxToDWord(int idx) {
	const static std::vector<std::vector<UCHAR>> instructions = {
	{ 0xb9 },		//ecx
	{ 0xba },		//rdx
	{ 0x41, 0xb8 }, //r8d
	{ 0x41, 0xb9 }  //r9d
	};
	return instructions[idx];
}


//movss xmm, rax
inline const std::vector<UCHAR>& ArgIdxToXMM32(int idx) {
	const static std::vector<std::vector<UCHAR>> instructions = {
	{ 0xf3, 0x0f, 0x10, 0x04, 0x24 }, //movss  xmm0,QWORD PTR [rsp] 
	{ 0xf3, 0x0f, 0x10, 0x0c, 0x24 },	
	{ 0xf3, 0x0f, 0x10, 0x14, 0x24 }, 
	{ 0xf3, 0x0f, 0x10, 0x1c, 0x24 }  
	};
	return instructions[idx];
}
//movsd xmm, rax
inline const std::vector<UCHAR>& ArgIdxToXMM64(int idx) {
	const static std::vector<std::vector<UCHAR>> instructions = {
	{ 0xf2, 0x0f, 0x10, 0x04, 0x24 }, //movsd  xmm0,QWORD PTR [rsp] 
	{ 0xf2, 0x0f, 0x10, 0x0c, 0x24 },
	{ 0xf2, 0x0f, 0x10, 0x14, 0x24 },
	{ 0xf2, 0x0f, 0x10, 0x1c, 0x24 }
	};
	return instructions[idx];
}
