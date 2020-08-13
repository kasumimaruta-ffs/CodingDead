// TestDll.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "TestDll.h"
#include <iostream>

using namespace std;

void TestClass::Function(void)
{
	cout << "TestClass.Function()" << endl;
	return;
}