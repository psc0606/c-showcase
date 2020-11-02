#include <iostream>
#include <string.h>

using namespace std;

class PERSON
{
public:
	PERSON();
	PERSON(const char *,const char *);
	PERSON(const PERSON&);
	//与类同名，无返回值，参数为该类型的引用，通常加入const
	void output(void);
	void input(void);
	void operator = (const PERSON& per);
	char name[30];
	char ID[30];
	~PERSON()
	{
		cout<<"析构函数"<<endl;
	}
};

void f(PERSON &per);
PERSON &f2(PERSON &per);

void f(PERSON &per)
{
	cout<<"in f():\n"<<per.name<<"  "<<per.ID<<endl;
}

PERSON &f2(PERSON &per)
{
	strcat(per.name,"f2");
	strcat(per.ID," f2");
	return per;
}

PERSON::PERSON()
{
	strcpy(name,"no");
	strcpy(ID,"0");
}

PERSON::PERSON(const char *n,const char *i)
{
	strcpy(name,n);
	strcpy(ID,i);
}

PERSON::PERSON(const PERSON &per)
{
	cout<<"\tcopy constructor\n";
	strcpy(name,per.name);
	strcpy(ID,per.ID);
}

void PERSON::output(void)
{
	cout<<"name:"<<name<<""<<"ID: "<<ID<<endl;
}

void PERSON::input(void)
{
	cout<<"enter name and id:\n";
	cin>>name>>ID;
}

void PERSON::operator = (const PERSON &per)
{
	cout<<"\toperator = \n";
	strcpy(name,per.name);
	strcpy(ID,per.ID);
}

int main()
{
	PERSON A;
	//A.output();
	
	PERSON B("B","002");
	//B.output();
	//A = PERSON("A","001");
	//A.output();
	//A = PERSON();
	//A.output();
	cout <<"\tPERSON C(B)\n";
	
	PERSON C(B);
	C.output();
	cout<<"\tC = A\n";
	C = A;
	C.output();
	cout<<"\tf(B)\n";
	f(B);
	PERSON D;
	cout<<"\tD=f2(B)"<<endl;
	D = f2(B);
	D.output();
}
