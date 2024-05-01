#include <Windows.h>
#include <iostream>
#include <filesystem>
#include "utils.h"
#include "code-gen.h"


#define LIB_NAME "Q:/GitHub/runtime-function-caller/x64/Release/sample-callee.dll"

using int64 = __int64;

//leaving this here so i can test and stuff
extern "C" __declspec(dllexport) int64 Func1(int64 a, int64 b, int64 c, int64 d, int64 e, int64 f, int64 g);
using pFunc1_t = decltype(&Func1);

int main() {
	int a = 0;
	float b = 0;
	//std::cout << std::filesystem::current_path();
	HMODULE hmod = LoadLibraryA(LIB_NAME);
	pFunc1_t pFunc1 = (pFunc1_t)GetProcAddress(hmod, "Func1");

	std::vector<QWORD> args = { 1, 2, 3, 4, 5, 6, 7 };
	std::vector<ArgType> args_type = { QWORD_t, QWORD_t };
	CodeGenerator generator(pFunc1, args, args_type);


	a = pFunc1(1, 2, 3, 4, 5, 6, 7);
	auto ptr = generator.GetCodePtr();
	std::cout << ptr() << std::endl;
	
}
