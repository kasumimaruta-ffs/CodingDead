#include <iostream>

using namespace std;

void Print()
{
	{
		int scopeFunction;
		int scopeFunctionCall;
		scopeFunctionCall = 0;
		cout << "scopeFunctionCall : " << scopeFunctionCall << endl;
	}
}

int main(void)
{
	Print();

	return 0;
}