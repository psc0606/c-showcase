#include <iostream>
#include <cmath>
using namespace std;

int main()
{
	double a = 88;
	double b = 0;
	b = sqrt(a);
	cout<<b<<endl;
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(5);
	cout<<b<<endl;
	cout<<static_cast<short>(b)<<endl;//强制类型转换
}
