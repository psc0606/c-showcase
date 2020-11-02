#include <iostream>

using namespace std;

class A
{
public:
	static int a;
	static void f()
	{
		cout<<"static function\n";
	}
};

int A::a = 1;//静态变量初始化

class B:public A
{
	
};

int main(int argc,char **argv)
{
	A x;
	cout<<x.a<<endl;
	++x.a;
	x.f(); //A::f();
	B y;
	cout<<y.a<<endl;
	y.f();
}
