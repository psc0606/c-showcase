#include <iostream>
using namespace std;

class TIME
{
public:
	TIME();
	void output();
	void input();
private:
	bool settime(int d,int m);
	int day;
	int month;
};

TIME::TIME():day(1),month(1)
{
	/*day = 1;
	month = 1;*/
}

void TIME::output()
{
	cout<<"month="<<month<<endl<<"day="<<day<<endl;
}

int main()
{
	TIME birthday;
	birthday.output();
	return 0;
}
