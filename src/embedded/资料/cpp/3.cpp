#include <iostream>
using namespace std;

void f(int a,int b,int c = 8,int d = 9)
{
	static int i = 0;
	i++;
	cout<<"output"<<i<<":"<<endl;
	cout<<"a="<<a<<endl<<"b="<<b<<endl
		<<"c="<<c<<endl<<"d="<<d<<endl;
}

int main()
{
	//f(1);
	f(1,2);
	f(1,2,3);
	f(1,2,3,4);
}
