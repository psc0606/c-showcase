#include"task2.h"
#include<stdio.h>
#include<stdlib.h>

void setNULL2()
{
	top2 =-1;
}

int is_empty2(void)
{
	return top2 == -1;
}

int is_full2(void)
{
	return top2 == (STACK_SIZE-1);
}

STACK_TYPE2 Top2(void)
{
	if( !is_empty2() )
	{
		return stack2[top2];
	}
	return 1;
}

int pop2(void)
{
	if( !is_empty2() )
	{
		top2 -= 1;
	}
	else
	{
		return 0;
	}
	return 1;
}

int push2(STACK_TYPE2 value)
{
	if( !is_full2() )
	{
		top2 += 1;
		stack2[top2] = value;
	}
	else
	{
		return 0;
	}
	return 1;
}

