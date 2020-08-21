#pragma once

#include <iostream>

extern "C" class __declspec(dllexport) TestClass
{
public:
	void Function(void)
	{
		using namespace std;
		cout << "TestClass.Function()" << endl;
		return;
	}

	void DeadFunction(void)
	{
		using namespace std;
		cout << "TestClass.Function()" << endl;
		return;
	}
};
