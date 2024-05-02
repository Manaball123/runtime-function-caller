#pragma once


using int64 = __int64;


extern "C" __declspec(dllexport) int64 Func1(int64 a, int64 b, int64 c, int64 d, int64 e, int64 f, int64 g);

extern "C" __declspec(dllexport) int64 Func2(int64 a, int b, double c, float d, int64 e, int64 f, int64 g);

extern "C" __declspec(dllexport) double Func3(double a, double b);