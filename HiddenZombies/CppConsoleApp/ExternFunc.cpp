#include <iostream>


void externFunction()
{
	std::cout << __FUNCSIG__ << "\n";
}

void externFunction_Z()
{
	std::cout << __FUNCSIG__ << "\n";
}

