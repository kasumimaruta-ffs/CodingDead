#include<iostream>

using namespace std;

class Parent 
{
public:
	virtual void Print()
	{
		cout << "Parent" << endl;
	}

	virtual void Print2()
	{
		cout << "Parent!" << endl;
	}
};

class Child : public Parent
{
public:
	void Print() override
	{
		cout << "Child" << endl;
	}

	void Print2() override
	{
		cout << "Child!" << endl;
	}
};

class DeadChild : public Parent
{
public:
	void Print() override
	{
		cout << "DeadChild" << endl;
	}

	void Print2() override
	{
		cout << "DeadChild!" << endl;
	}
};

int main()
{
	Parent* polymorphism[2] = { new Parent,
								new Child };

	Parent* polymorphismDead[2] = { new Parent,
									new DeadChild };

	for( int i = 0; i < sizeof(polymorphism)/ sizeof(polymorphism[0]); i++ )
	{
		polymorphism[i]->Print();
		delete polymorphism[i];
	}

	return 0;
}