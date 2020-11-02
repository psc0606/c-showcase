#include <stdio.h>
#include <stdarg.h>
float average(int n_value,...)
{
	va_list var_arg;
	int count;
	float sum = 0;
	va_start(var_arg,n_value);
	for (count = 0;count < n_value - 1;count++)
	{
		sum += va_arg(var_arg,double);
	}
	sum += va_arg(var_arg,int);
	va_end(var_arg);
	return sum/n_value;
}
int main()
{
	printf("%f",average(5,3.5,4.3,6.2,4.0,50));
}

