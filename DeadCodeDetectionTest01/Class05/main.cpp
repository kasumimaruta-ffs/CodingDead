#include <iostream>
using namespace std;

class Base
{
public:
	virtual void GetName();
};

class Sub : public Base
{
public:
	void GetName() override;
};

void Base::GetName()
{
	cout << "Base" << endl;
	return;
}

void Sub::GetName()
{
	cout << "Sub" << endl;
	return;
}

class BaseCall
{
public:
	virtual void GetName();
};

class SubCall : public BaseCall
{
public:
	void GetName() override;
};

void BaseCall::GetName()
{
	cout << "Base" << endl;
	return;
}

void SubCall::GetName()
{
	cout << "Sub" << endl;
	return;
}

int main(void)
{
	BaseCall* baseCall = new BaseCall();
	baseCall->GetName();

	BaseCall* subCall = new SubCall();
	subCall->GetName();

	delete(baseCall);
	delete(subCall);

	return 0;
}