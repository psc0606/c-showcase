#include <iostream>

double ave(double &n1,double &n2);
double ave(double n1,double n2);
double ave(double n1,double n2,double n3);

int main()
{
	using namespace std;
	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(2);
	
	cout<<"the average of 2.1,2.5 and 3.0 is :"<<ave(2.1,2.5,3.0)<<endl;
	cout<<"the average of 2.1,2.5 is :"<<ave(2.1,2.5)<<endl;
	
	return 0;
}

double ave(double n1,double n2)
{
	return (n1+n2)/2.0;
}

double ave(double n1,double n2,double n3)
{
	return (n1+n2+n3)/3.0;
}
