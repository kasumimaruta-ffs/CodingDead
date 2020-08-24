// TestDll.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "TestDll.h"
#include <iostream>

using namespace std;

extern "C" void __declspec(dllexport) DllFunctionTest()
{
	cout << "Call Dll Function." << std::endl;
	return;
}

extern "C" void __declspec(dllexport) DeadDllFunctionTest()
{
	cout << "Call Dll Function." << std::endl;
	return;
}
