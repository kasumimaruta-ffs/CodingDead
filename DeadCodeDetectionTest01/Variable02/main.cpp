#include <iostream>

using namespace std;

int globalVariable;
int globalVariableCall;

int main(void)
{

	globalVariableCall = 0;

	cout << "globalVariableCall : " << globalVariableCall << endl;

	return 0;
}