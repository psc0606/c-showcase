#include <iostream>

using namespace std;

class A
{
public:
	A(int a = 0, int b = 0)
	{
		this->a = a;
		this->b = b;
	}
protected:
	int a;
	int b;
};

class B:protected A
{
public:
	int c;
	int d;
	B(int c = 0, int d = 0)
	{
		this->c = c;
		this->d = d;
	}
	
	void output()
	{
		cout<<"a:"<<a<<endl
			<<"b:"<<b<<endl
			<<"c:"<<c<<endl
			<<"d:"<<d<<endl;
	}
};

class C:protected B
{
	void output()
	{
		cout<<"a:"<<a<<endl;
	}
};

int main()
{
	B b;
	b.c = 11;
	b.output();
}
