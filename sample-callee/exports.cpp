#include "pch.h"




int64 Func1(int64 a, int64 b, int64 c, int64 d, int64 e, int64 f, int64 g) {
	return a + b + c + d + e + f + g;
}
/*
0:  48 8d 04 11             lea    rax,[rcx+rdx*1]
4:  49 03 c0                add    rax,r8
7:  49 03 c1                add    rax,r9
a:  48 03 44 24 28          add    rax,QWORD PTR [rsp+0x28]
f:  48 03 44 24 30          add    rax,QWORD PTR [rsp+0x30]
14: 48 03 44 24 38          add    rax,QWORD PTR [rsp+0x38]
*/

float Func2(float a, float b) {
	return a + b;
}

double Func3(double a, double b) {
	return a + b;
}
