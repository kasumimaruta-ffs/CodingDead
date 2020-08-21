#include<iostream>

using namespace std;

class Parent 
{
public:
	virtual void Print()
	{
		cout << "Parent" << endl;
	}
};

class Child : public Parent
{
public:
	void Print() override
	{
		cout << "Child" << endl;
	}
};

class DeadParent
{
public:
	virtual void Print()
	{
		cout << "Parent" << endl;
	}
};

class DeadChild : public DeadParent
{
public:
	void Print() override
	{
		cout << "Child" << endl;
	}
};

int main()
{
	Parent* polymorphism[2] = { new Parent,
								new Child };

	for( int i = 0; i < sizeof(polymorphism)/ sizeof(polymorphism[0]); i++ )
	{
		polymorphism[i]->Print();
		delete polymorphism[i];
	}

	return 0;
}