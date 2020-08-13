#include <iostream>

using namespace std;

void TestFunction()
{
	cout << "TestFunction" << endl;
}

void WrapperFunction()
{
	cout << "TestFunction Wrapper" << endl;
	TestFunction();
}

int main()
{
	WrapperFunction();
	return 0;
}