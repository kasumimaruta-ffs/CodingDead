#include <iostream>

using namespace std;

class Variable
{
public:
	void Print();
};

void Variable::Print()
{
	cout << "Print" << endl;
}

int main(void)
{
	Variable variable;

	return 0;
}