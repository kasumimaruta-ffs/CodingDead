#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <functional>

using namespace std;

class TestClass {
public:
	TestClass()
	{
	}

	void Function() 
	{
		cout << "Function" << endl;
		return;
	}

	void DeadFunction()
	{
		cout << "Function" << endl;
		return;
	}
};

int main() {
	TestClass test;
	mem_fun(&TestClass::Function)(&test);


	vector<TestClass> vector(3);
	for_each(vector.begin(), vector.end(), mem_fun_ref(&TestClass::Function));
	return 0;

	return 0;
}