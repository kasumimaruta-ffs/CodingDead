#pragma once

class Singleton {
private:
	Singleton();
public:
	static Singleton& GetInstance();
	void Print();
};

class SingletonCall {
private:
	SingletonCall();
public:
	static SingletonCall& GetInstance();
	void Print();
};