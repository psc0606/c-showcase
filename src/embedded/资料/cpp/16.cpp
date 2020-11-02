#include <iostream>

using namespace std;

class A
{
public:
	int a;
	string sa;
	//如果定义了拷贝构造函数，编译器也不会合成默认构造函数
	A(int ax =1,string sax = "ABC"):a(ax),sa(sax)
	{
		cout<<"constructor A"<<endl;
	}
	A(A &x):a(x.a),sa(x.sa)
	{
		cout<<"copy constructor A"<<endl;
	}
};

class B:public A
{
public:
	int b;
	string sb;
	B(int ax = 1,string sax = "ABC",int bx = 3,string sbx = "def"):A(ax,sax),b(bx),sb(sbx)
	{
		cout<<"constructor B"<<endl;
	}
	B(B &x):A(x),b(x.b),sb(x.sb)
	//注意基类拷贝构造函数的调用参数是派生类对象
	//B(B &x):A(x),b(x.b),sb(x.sb)
	//如果不调用基类的拷贝构造函数，则将只复制派生类部分
	//而基类部分，调用的是基类的构造函数，而不是拷贝构造函数
	{
		cout<<"copy constructor B"<<endl;
	}
};

int main()
{
	B x(11,"xxx",22,"yyy");
	B y(x);
	cout<<y.a<<" "<<y.sa<<endl
		<<y.b<<" "<<y.sb<<endl;
}
