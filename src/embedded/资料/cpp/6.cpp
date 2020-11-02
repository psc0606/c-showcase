#include <iostream>
using namespace std;
/*struct TIME
{
	int month;
	int day;
	void output(void);
};
*/
class TIME
{
	public:
		int month;
		int day;
		void output(void);
		void input(void);
};

int main()
{
	//struct TIME birthday;
	class TIME birthday;
	cout<<"enter the month:\n";
	cin>>birthday.month;
	cout<<"enter the day:\n";
	cin>>birthday.day;
	birthday.output();
	cout<<sizeof(struct TIME)<<endl;
}

void TIME::output(void)
{
	cout<<"month="<<month<<endl;
	cout<<"dya="<<day<<endl;
}
