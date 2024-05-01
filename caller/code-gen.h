#pragma once
#include <Windows.h>
#include <vector>
#include "utils.h"

using QWORD = unsigned __int64;
/*
0:  48 c7 c0 01 00 00 00    mov    rax,0x1
7:  48 c7 c3 01 00 00 00    mov    rbx,0x1
e:  48 c7 c1 01 00 00 00    mov    rcx,0x1
15: 48 c7 c2 01 00 00 00    mov    rdx,0x1
1c: 48 c7 c5 01 00 00 00    mov    rbp,0x1
23: 48 c7 c4 01 00 00 00    mov    rsp,0x1
2a: 48 c7 c6 01 00 00 00    mov    rsi,0x1
31: 48 c7 c7 01 00 00 00    mov    rdi,0x1
38: 6a 01                   push   0x1
3a: ff d0                   call   rax 
*/

/*
0:  48 b8 ff ff ff ff ff    movabs rax,0xffffffffffffff
7:  ff ff 00
a:  48 bb ff ff ff ff ff    movabs rbx,0xffffffffffffff
11: ff ff 00
14: 48 b9 ff ff ff ff ff    movabs rcx,0xffffffffffffff
1b: ff ff 00
1e: 48 ba ff ff ff ff ff    movabs rdx,0xffffffffffffff
25: ff ff 00
28: 48 be ff ff ff ff ff    movabs rsi,0xffffffffffffff
2f: ff ff 00
32: 48 bf ff ff ff ff ff    movabs rdi,0xffffffffffffff
39: ff ff 00
3c: 48 bd ff ff ff ff ff    movabs rbp,0xffffffffffffff
43: ff ff 00
46: 48 bc ff ff ff ff ff    movabs rsp,0xffffffffffffff
4d: ff ff 00
50: 49 b8 ff ff ff ff ff    movabs r8,0xffffffffffffff
57: ff ff 00
5a: 49 b9 ff ff ff ff ff    movabs r9,0xffffffffffffff
61: ff ff 00
*/

/*
0:  f2 0f 10 00             movsd  xmm0,QWORD PTR [rax]
4:  f2 0f 10 08             movsd  xmm1,QWORD PTR [rax]
8:  f2 0f 10 10             movsd  xmm2,QWORD PTR [rax]
c:  f2 0f 10 18             movsd  xmm3,QWORD PTR [rax]
*/


QWORD SampleFunc();
using code_t = decltype(SampleFunc);
using pCode_t = decltype(&SampleFunc);

inline void WriteToBlock(UCHAR* block, size_t& offset, UCHAR val) {

	*(block + offset) = val;
	offset++;
}

inline void WriteToBlock(UCHAR* block, size_t& offset, UCHAR* val, size_t length = 1, bool reverse_endianness = false) {
	for (size_t i = 0; i < length; i++) {
		*(block + offset + i) = *(val + i);
	}
	offset += length;
}
inline void WriteMovabsRAX(UCHAR* block, size_t& offset, QWORD val) {
	short movabs_rax = 0xb848;
	WriteToBlock(block, offset, (UCHAR*)&movabs_rax, 2);
	WriteToBlock(block, offset, (UCHAR*)&val, 8);
}

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

enum ArgType {
	DWORD_t,
	QWORD_t,
	FLOAT_t,
	DOUBLE_t,
};

class CodeGenerator {
	UCHAR* code_block;

	inline void Init(void* address, std::vector<QWORD> args, std::vector<ArgType> arg_types) {
		GenCode((QWORD)address, args);
	}
	void GenCode(QWORD address, std::vector<QWORD> args);

public:
	pCode_t GetCodePtr() {
		return (pCode_t)code_block;
	}
	CodeGenerator(void* address, std::vector<QWORD> args, std::vector<ArgType> arg_types) {
		Init(address, args, arg_types);
	}
};



//func1(int a, int b, int c, int d, int e, int f);
// a in RCX, b in RDX, c in R8, d in R9, f then e pushed on stack

//