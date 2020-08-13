#include <stdio.h>
#include <iostream>
#include "Header.h"

using namespace std;

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