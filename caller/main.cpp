#include <Windows.h>
#include <iostream>
#include <filesystem>
#include "utils.h"
#include "code-gen.h"


#define LIB_NAME "Q:/GitHub/runtime-function-caller/x64/Release/sample-callee.dll"

using int64 = __int64;

//leaving this here so i can test and stuff

extern "C" __declspec(dllexport) int64 Func1(int64 a, int64 b, int64 c, int64 d, int64 e, int64 f, int64 g);

extern "C" __declspec(dllexport) int64 Func2(int64 a, int b, double c, float d, int64 e, int64 f, int64 g);
using pFunc1_t = decltype(&Func1);
using pFunc2_t = decltype(&Func2);
int main() {
	int a = 0;
	float b = 0;
	//std::cout << std::filesystem::current_path();
	HMODULE hmod = LoadLibraryA(LIB_NAME);
	pFunc1_t pFunc2 = (pFunc1_t)GetProcAddress(hmod, "Func2");
	double v3 = 3.0;
	float v4 = 4.0;
	std::vector<QWORD> args = { 1, 2, *(QWORD*)&v3, *(QWORD*)&v4, 5, 6, 7 };
	std::vector<ArgType> args_type = { QWORD_t, DWORD_t, DOUBLE_t, FLOAT_t, QWORD_t, QWORD_t, QWORD_t };
	CodeGenerator generator(pFunc2, args, args_type);

	const char* arga = "hello";
	const char* argb = " world";
	std::vector<QWORD> args2 = { arga, argb };
	std::vector<ArgType> args_type2 = { QWORD_t, QWORD_t};
	
	CodeGenerator gen2(&strcat, args2, args_type2);
	std::cout << (const char*)gen2.GetCodePtr() << std::endl;

	a = pFunc2(1, 2, 3, 4, 5, 6, 7);
	auto ptr = generator.GetCodePtr();
	std::cout << ptr() << std::endl;
	
}
