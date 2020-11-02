#include <iostream>
using namespace std;

int main()
{
	int *p = NULL;
	p = new int;
	
	cout <<"please input a integer:\n";
	cin >> *p;
	cout<<"the integer you have input is:";
	cout<<*p<<endl;
	delete p;
	
	p = new int[5];
	int i,j;
	for (i = 0; i < 5; i++)
	{
		cin>>p[i];
	}
	cout<<"the integer you have input is :\n";
	for(i = 0; i < 5; i++)
	{
		cout<<p[i]<<endl;
	}
	delete []p;
	
	int **q = NULL;
	q = new int*[5];
	for (i = 0; i < 5; i++)
	{
		q[i] = new int[5];
		for (j = 0; j < 5; j++)
		{	
			q[i][j] = i*(j+1);
		}
	}
	
	cout<<"the integer you have input is :\n";
	for(i = 0; i < 5; i++)
	{
		for (j = 0; j < 5; j++)
		{
			cout<<q[i][j]<<endl;
		}
	}
	
	for (i = 0; i < 5; i++)
	{
		delete q[i];
	}
	delete q;
}
