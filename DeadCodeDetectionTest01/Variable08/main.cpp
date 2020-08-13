#include <iostream>

using namespace std;

class Variable
{
public:
	void Print();
	int variable;
};

void Variable::Print()
{
	cout << "Variable" << endl;
}

class VariableCall
{
public:
	void Print();
	int variableCall;
};

void VariableCall::Print()
{
	cout << "VariableCall" << variableCall << endl;
}

int main(void)
{
	VariableCall variableCall;
	variableCall.Print();

	return 0;
}