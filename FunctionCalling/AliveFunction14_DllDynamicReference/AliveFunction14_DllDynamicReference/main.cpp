#include <iostream>
#include "TestDll.h"
#include <windows.h>

typedef void(*FUNC)();

int main()
{
	// DLL�Ăяo��
	HMODULE hModule = LoadLibrary("TestDll.dll");

	if (NULL == hModule)
	{
		return -1;
	}

	// �֐��ǂݍ���
	FUNC DllFunctionTest = (FUNC)GetProcAddress(hModule, "DllFunctionTest");

	// �֐��R�[�����s
	DllFunctionTest();

	return 0;
}