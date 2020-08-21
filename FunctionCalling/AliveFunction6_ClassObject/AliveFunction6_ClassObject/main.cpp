#include <iostream>

using namespace std;

class ClassObject
{
public:
	void Print()
	{
		cout << "Print Test." << endl;
		return;
	}

	void PrintPointer()
	{
		cout << "Print Test Pointer." << endl;
		return;
	}

	void DeadPrint()
	{
		cout << "Print Test." << endl;
		return;
	}

	void DeadPrintPointer()
	{
		cout << "Print Test Pointer." << endl;
		return;
	}
};

int main()
{
	ClassObject object;
	object.Print();

	ClassObject* objectPointer = new ClassObject(); ////////
	objectPointer->PrintPointer();

	return 0;
}