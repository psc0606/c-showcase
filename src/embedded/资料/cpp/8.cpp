#include <iostream>
#include <string>
using namespace std;

int main()
{
	string s1("hello");
	string s2(" world");
	string s3(s1);
	string s4(10,'c');
	s1 = s1+s2;//必须有一个是string类型对象
	if (!s1.empty())
	{
		cout<<"s1:"<<s1<<endl<<"s1 size="<<s1.size()<<endl;
	}
	cout<<"s1[1]="<<s1[1]<<endl;
	cout<<"s4="<<s4<<endl;
	if (s1 > s2)
	{
		cout<<"s1>s2\n";
	}
	return 0;
}
