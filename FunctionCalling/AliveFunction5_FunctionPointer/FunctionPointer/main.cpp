#include <iostream>

using namespace std;

int Exponentiation(int x)
{
	return x*x;
}

int DeadExponentiation(int x)
{
	return x*x;
}

int main()
{
	int(*functionPointer)(int) = Exponentiation;

	int a = 3;

	cout <<  (*functionPointer)(a) << endl;
	return 0;
}