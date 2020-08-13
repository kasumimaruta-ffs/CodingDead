#include <iostream>

using namespace std;

void Print()
{
	cout << "Print" << endl;
}

int main(void)
{
	bool flag = true;

	if (true == flag)
	{
	}
	else
	{
		Print();
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