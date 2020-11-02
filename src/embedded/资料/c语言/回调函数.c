#include <stdio.h>
#define ADD 0
#define SUB 1
#define MUL 2
#define DIV 3

double add(double x, double y)
{
	return x + y;
}

double sub(double x, double y)
{
	return x - y;
}

double mul(double x, double y)
{
	return x * y;
}

double div(double x, double y)
{
	if (0 == y)
	{
		printf("err\n");
		return -1;
	}
	return x / y;
}

void main()
{
	double (*oper_func[])(double,double) =
					 {add,sub,mul,div};
	int oper = 1;
	double result = oper_func[oper](1.5,3.6);
	printf("result is %lf\n",result);
}
