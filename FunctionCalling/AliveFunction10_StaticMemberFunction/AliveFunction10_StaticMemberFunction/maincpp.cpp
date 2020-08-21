#include<iostream>

using namespace std;

class StaticMemmber
{
public:
	static void Print()
	{
		cout << "Static Member Function." << endl;
		return;
	}

	static void DeadPrint()
	{
		cout << "Static Member Function." << endl;
		return;
	}
};

int main()
{
	void(*functionPointer)() = StaticMemmber::Print;

	functionPointer();
	return 0;
}