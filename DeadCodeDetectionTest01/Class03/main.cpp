#include <stdio.h>
#include <iostream>

using namespace std;

class Singleton {
private:
	Singleton();
public:
	static Singleton& GetInstance();
	void Print();
};

Singleton::Singleton()
{
}

Singleton& Singleton::GetInstance()
{
	static Singleton instance;
	return instance;
}

void Singleton::Print()
{
	cout << "Print" << endl;
}

class SingletonCall {
private:
	SingletonCall();
public:
	static SingletonCall& GetInstance();
	void Print();
};

SingletonCall::SingletonCall()
{
}

SingletonCall& SingletonCall::GetInstance()
{
	static SingletonCall instance;
	return instance;
}

void SingletonCall::Print()
{
	cout << "Print" << endl;
}

int main()
{
	SingletonCall::GetInstance().Print();

	return 0;
}