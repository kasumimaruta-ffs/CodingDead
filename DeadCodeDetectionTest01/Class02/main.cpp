#include <iostream>
#include "Header.h"

using namespace std;

void Variable::Print()
{
	cout << "Print" << endl;
}

void VariableCall::Print()
{
	cout << "VariableCall Print" << endl;
}

int main(void)
{
	Variable variable;
	VariableCall variableCall;
	variableCall.Print();

	return 0;
}