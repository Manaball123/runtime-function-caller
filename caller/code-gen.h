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
0:  f2 0f 10 04 24          movsd  xmm0,QWORD PTR [rsp]
5:  f2 0f 10 0c 24          movsd  xmm1,QWORD PTR [rsp]
a:  f2 0f 10 14 24          movsd  xmm2,QWORD PTR [rsp]
f:  f2 0f 10 1c 24          movsd  xmm3,QWORD PTR [rsp]
14: f3 0f 10 04 24          movss  xmm0,DWORD PTR [rsp]
19: f3 0f 10 0c 24          movss  xmm1,DWORD PTR [rsp]
1e: f3 0f 10 14 24          movss  xmm2,DWORD PTR [rsp]
23: f3 0f 10 1c 24          movss  xmm3,DWORD PTR [rsp]
*/


QWORD SampleFunc();
using code_t = decltype(SampleFunc);
using pCode_t = decltype(&SampleFunc);


enum ArgType {
	DWORD_t,
	QWORD_t,
	FLOAT_t,
	DOUBLE_t,
};

class CodeGenerator {
	UCHAR* code_buf;
	size_t cur_offset = 0;
	inline void Init(void* address, const std::vector<QWORD>& args, const std::vector<ArgType>& arg_types) {
		GenCode((QWORD)address, args, arg_types);
	}
	void GenCode(QWORD address, const std::vector<QWORD>& args, const std::vector<ArgType>& arg_types);

	inline void WriteByte(UCHAR val) {
		*(code_buf + cur_offset) = val;
		cur_offset++;
	}
	inline void WriteBytes(std::vector<UCHAR> val) {
		for(UCHAR c : val){
			*(code_buf + cur_offset) = c;
			cur_offset++;
		}
	}
	template <typename T>
	inline void WriteVal(T val) {
		*reinterpret_cast<T*>(code_buf + cur_offset) = val;
		cur_offset += sizeof(val);
	}
	inline void WriteDataBlock(UCHAR* val, size_t length = 1) {
		for (size_t i = 0; i < length; i++) {
			*(code_buf + cur_offset + i) = *(val + i);
		}
		cur_offset += length;
	}
	inline void WriteMovabsRAX(QWORD val) {
		WriteBytes({ 0x48, 0xb8 });
		WriteVal(val);
	}
	inline void WritePushQWord(QWORD val) {
		WriteMovabsRAX(val);
		//push rax
		WriteByte(0x50);
	}
	inline void WritePushDWord(DWORD val) {
		WriteByte(0x68);
		WriteVal(val);
	}
	inline void WritePushRSP(){ WriteByte(0x54); }
	inline void WritePopRSP() { WriteByte(0x5c); }
	//not too sure if its signed/unsigned actually but probably wont ever be a problem
	//0:  48 81 ec ff 00 00 00    sub    rsp,0xff 
	inline void WriteSubRsp(int val) {
		WriteBytes({ 0x48, 0x81, 0xec });
		WriteVal(val);
	}
	inline void WriteAddRsp(int val) {
		WriteBytes({ 0x48, 0x81, 0xc4 });
		WriteVal(val);
	}
	inline void WriteArgToRegister(int arg_idx, QWORD val, ArgType type) {
		switch (type) {
		case QWORD_t:
			WriteBytes(ArgIdxToQWord(arg_idx));
			WriteVal(val);
			return;
		case DWORD_t:
			WriteBytes(ArgIdxToDWord(arg_idx));
			WriteVal((DWORD)val);
			return;
		case FLOAT_t:
			//first push on stack
			WritePushDWord(val);
			WriteBytes(ArgIdxToXMM32(arg_idx));
			//I know its weird but its extended to 64bits, see https://stackoverflow.com/questions/40305965/does-each-push-instruction-push-a-multiple-of-8-bytes-on-x64
			WriteAddRsp(8);
			return;
		case DOUBLE_t:
			//first push on stack
			WritePushQWord(val);
			WriteBytes(ArgIdxToXMM64(arg_idx));
			WriteAddRsp(8);
			return;
		}
	}
	//note that order matters here
	inline void WriteArgToStack(QWORD val, ArgType type) {
		switch (type) {
		case QWORD_t:
			WritePushQWord(val); return;
		case DWORD_t:
			WritePushDWord(val); return;
		case FLOAT_t:
			WritePushDWord(val); return;
		case DOUBLE_t:
			WritePushQWord(val); return;
		}
	}
	inline void WriteCallRAX() {
		WriteBytes({ 0xff, 0xd0 });
	}
public:
	pCode_t GetCodePtr() {
		return (pCode_t)code_buf;
	}
	CodeGenerator(void* address, const std::vector<QWORD>& args, const std::vector<ArgType>& arg_types) {
		Init(address, args, arg_types);
	}
};



//func1(int a, int b, int c, int d, int e, int f);
// a in RCX, b in RDX, c in R8, d in R9, f then e pushed on stack

//