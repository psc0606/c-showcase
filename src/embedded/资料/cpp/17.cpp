#include <iostream>

using namespace std;

class A
{
public:
	virtual void f()
	{
		cout<<"A"<<endl;
	}
};

class B:public A
{
public:
	void f()
	{
		cout<<"B"<<endl;
	}
};

void f1(A &x)
{
	x.f();
}

void f2(A x)//强制类型转换
{
	x.f();
}

void f3(A *px)
{
	px->f();
}

int main()
{
	A a;
	B b;
	f1(a);
	f1(b);
	f2(a);
	f2(b);
	f3(&a);
	f3(&b);
}
