#include <iostream>
#include "../TestDll/TestDll.h"
#include <windows.h>

typedef void(*FUNC)();

int main()
{
	// DLL呼び出し
	HMODULE hModule = LoadLibrary("TestDll.dll");

	if (NULL == hModule)
	{
		return -1;
	}

	// 関数読み込み
	FUNC DllFunctionTest = (FUNC)GetProcAddress(hModule, "DllFunctionTest");

	// 関数コール実行
	DllFunctionTest();

	return 0;
}