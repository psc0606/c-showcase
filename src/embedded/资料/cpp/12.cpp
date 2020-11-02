#include <iostream>
using namespace std;

class CPoint
{
public:
	CPoint(int newx = 0,int newy = 0):x(newx),y(newy){}
	int get_x() const{ return x;}
	int get_y() const{ return y;}
	void set_x(int newx)
	{
		x = newx;
	}
	void set_y(int newy)
	{
		y = newy;
	}
	void output()
	{
		cout<<"("<<x<<","<<y<<")"<<endl;
	}
	CPoint operator -(CPoint &point);
	CPoint &operator++();
	CPoint operator++(int);
	friend CPoint operator+(CPoint &A,CPoint &B);
	friend void operator<<(ostream &c_out,CPoint &point);
	
private:
	int x;
	int y;
};

CPoint CPoint::operator-(CPoint &point)
{
	cout<<"operator-"<<endl;
	CPoint C;
	C.x = this->x-point.x;
	C.y = this->y-point.y;
	return C;
}

CPoint operator+(CPoint &A,CPoint &B)
{
	cout<<"operator+"<<endl;
	CPoint C;
	C.x = A.x + B.x;
	C.y = A.y + B.y;
	return C;
}

void operator<<(ostream &c_out,CPoint &point)
{
	c_out<<"("<<point.x<<","<<point.y<<")"<<endl;
}

CPoint &CPoint::operator++()
{
	this->x+=1;
	this->y+=1;
	return *this;
}

CPoint CPoint::operator++(int)
{
	CPoint temp = *this;
	this->x+=1;
	this->y+=1;
	return temp;
}

int main(int argc,char **argv)
{
	CPoint a(1,2),b(3,4);
	CPoint c;
	c = a + b;
	cout<<a;
	cout<<b;
	cout<<c;
	c = a - b;
	cout<<c;
	c++;
	cout<<c;
	++b;
	cout<<b;
}
