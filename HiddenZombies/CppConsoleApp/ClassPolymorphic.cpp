#include "ClassPolymorphic.h"


std::unique_ptr<abstructclass> classPolymorphicFactory(int p)
{
	switch (p)
	{
	case 0:
		return std::unique_ptr<abstructclass>(new baseclass());
	case 1:
		return std::unique_ptr<abstructclass>(new subclass1());
	}
}

void abstructclass::memFunc()
{
	std::cout << __FUNCSIG__ << "\n";
}

void abstructclass::memFunc_Z()
{
	std::cout << __FUNCSIG__ << "\n";
}



void baseclass::memFunc()
{
	std::cout << __FUNCSIG__ << "\n";
}

void baseclass::memFunc_Z()
{
	std::cout << __FUNCSIG__ << "\n";
}



void subclass1::memFunc()
{
	std::cout << __FUNCSIG__ << "\n";
}

void subclass1::memFunc_Z()
{
	std::cout << __FUNCSIG__ << "\n";
}

void subclass1::staticMemberFunc()
{
	std::cout << __FUNCSIG__ << "\n";
}

void subclass1::staticMemberFunc_Z()
{
	std::cout << __FUNCSIG__ << "\n";
}



void subclass2::memFunc()
{
	std::cout << __FUNCSIG__ << "\n";
}

void subclass2::memFunc_Z()
{
	std::cout << __FUNCSIG__ << "\n";
}
