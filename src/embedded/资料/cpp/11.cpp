#include <iostream>
using namespace std;

class B;
class A
{
public:
	A():a(19)
	{}
private:
	int a;
	friend class B;
};

class B
{
public:
	int b;
	void output(A &x)
	{
		cout<<x.a<<endl;
	}
};

int main(int argc,char **argv)
{
	B b_obj;
	A a_obj;
	b_obj.output(a_obj);
}
