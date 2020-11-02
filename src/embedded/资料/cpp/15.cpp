#include <iostream>

using namespace std;

class A
{
public:
	int a;
	A(int x = 0):a(x)
	{
		cout<<"A constructor"<<endl;
	}
	
	~A()
	{
		cout<<"destractor A"<<endl;
	}
};

class B:public A
{
public:
	int b;
	B(int x = 1,int y = 2):A(x),b(y)
	{
		cout<<"B constructor"<<endl;
	}
	~B()
	{
		cout<<"destructor B"<<endl;
	}
};

int main()
{
	B x;
}
