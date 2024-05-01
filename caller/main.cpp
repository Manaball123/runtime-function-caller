#include <Windows.h>
#include <iostream>
#include <filesystem>
#include "utils.h"
#include "code-gen.h"


#define LIB_NAME "Q:/GitHub/runtime-function-caller/x64/Release/sample-callee.dll"

int main() {
	int a = 0;
	float b = 0;
	//std::cout << std::filesystem::current_path();
	HMODULE hmod = LoadLibraryA(LIB_NAME);
	void* pFunc1 = GetProcAddress(hmod, "Func1");
	void* pFunc3 = GetProcAddress(hmod, "Func3");
	std::vector<QWORD> args = { 1,2 };
	pCode_t code = GenCode((QWORD)pFunc1, args);
	std::cout << code() << std::endl;
	
}
