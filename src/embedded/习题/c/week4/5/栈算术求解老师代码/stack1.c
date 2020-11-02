#include"task1.h"
#include<stdio.h>
#include<stdlib.h>

void setNULL1()
{
	top1 =-1;
}

int is_empty1(void)
{
	return top1 == -1;
}

int is_full1(void)
{
	return top1 == (STACK_SIZE-1);
}

STACK_TYPE1 Top1(void)
{
	if( !is_empty1() )
	{
		return stack1[top1];
	}
	return 1;
}

int pop1(void)
{
	if( !is_empty1() )
	{
		top1 -= 1;
	}
	else
	{
		return 0;
	}
	return 1;
}

int push1(STACK_TYPE1 value)
{
	if( !is_full1() )
	{
		top1 += 1;
		stack1[top1] = value;
	}
	else
	{
		return 0;
	}
	return 1;
}

