#include <iostream>

using namespace std;

void Print()
{
	cout << "Print" << endl;
}

int main(void)
{
	bool flag = false;

	if (true == flag)
	{
		Print();
	}
	else
	{
	}

	if (true == flag)
	{
		Print();
	}
	else
	{
		Print();
	}

	return 0;
}