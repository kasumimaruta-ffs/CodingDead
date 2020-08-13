#include <iostream>


void inlineFunction()
{
	std::cout << __FUNCSIG__ << "\n";
}

void inlineFunction_Z()
{
	std::cout << __FUNCSIG__ << "\n";
}

static void staticFunction()
{
	std::cout << __FUNCSIG__ << "\n";
}

static void staticFunction_Z()
{
	std::cout << __FUNCSIG__ << "\n";
}


#pragma once
