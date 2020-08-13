// CppDll.cpp : DLL 用にエクスポートされる関数を定義します。
//

#include "pch.h"
#include "framework.h"
#include "CppDll.h"
#include <iostream>


// これは、エクスポートされた変数の例です
CPPDLL_API int nCppDll=0;

// これは、エクスポートされた関数の例です。
CPPDLL_API void exportedDllFunction_1()
{
	std::cout << __FUNCSIG__ << "\n";
}

CPPDLL_API void exportedDllFunction_2()
{
	std::cout << __FUNCSIG__ << "\n";
}

CPPDLL_API void exportedDllFunction_Z_1()
{
	std::cout << __FUNCSIG__ << "\n";
}

CPPDLL_API void exportedDllFunction_3()
{
	std::cout << __FUNCSIG__ << "\n";
}

CPPDLL_API void exportedDllFunction_4()
{
	std::cout << __FUNCSIG__ << "\n";
}

CPPDLL_API void exportedDllFunction_Z_3()
{
	std::cout << __FUNCSIG__ << "\n";
}

// これは、エクスポートされたクラスのコンストラクターです。
ExportedClassA::ExportedClassA()
{
    return;
}
