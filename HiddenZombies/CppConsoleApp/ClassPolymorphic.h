#pragma once
#include <iostream>
#include <memory>


class abstructclass
{
public:
	virtual void memFunc() = 0;
	virtual void memFunc_Z() = 0;
};

class baseclass : public abstructclass
{
public:
	virtual void memFunc();
	virtual void memFunc_Z();
};

class subclass1 : public baseclass
{
public:
	virtual void memFunc();
	virtual void memFunc_Z();

	static void staticMemberFunc();
	static void staticMemberFunc_Z();
};

class subclass2 : public baseclass
{
public:
	virtual void memFunc();
	virtual void memFunc_Z();
};

std::unique_ptr<abstructclass> classPolymorphicFactory(int p);
