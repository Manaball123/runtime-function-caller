#include <Windows.h>
#include <iostream>
#include <filesystem>
#include "utils.h"
#include "code-gen.h"


#define LIB_NAME "Q:/GitHub/runtime-function-caller/x64/Release/sample-callee.dll"
extern "C" __declspec(dllexport) int Func1(int a, int b);
extern "C" __declspec(dllexport) float Func2(float a, float b);
extern "C" __declspec(dllexport) double Func3(double a, double b);
using pFunc1_t = decltype(&Func1);
using pFunc2_t = decltype(&Func2);
using pFunc3_t = decltype(&Func3);




int main() {
	int a = 0;
	float b = 0;
	//std::cout << std::filesystem::current_path();
	HMODULE hmod = LoadLibraryA(LIB_NAME);
	pFunc1_t pFunc1 = (pFunc1_t)GetProcAddress(hmod, "Func1");
	pFunc3_t pFunc3 = (pFunc3_t)GetProcAddress(hmod, "Func3");
	std::vector<QWORD> args = { 1,2 };
	pCode_t code = GenCode((QWORD)pFunc1, args);
	std::cout << code() << std::endl;

	a = pFunc1(1, 2);
	
}
