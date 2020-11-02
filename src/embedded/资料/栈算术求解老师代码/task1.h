#ifndef TASK1_H

#define TASK1_H
/*
**栈的模块接口
*/
typedef int STACK_TYPE1; // 栈所存储的值的类型

#define STACK_SIZE 50

STACK_TYPE1 stack1[STACK_SIZE];
static int top1 = -1;

/*
** value压入栈
*/
int push1(STACK_TYPE1 value);

/*
**出栈  并将其丢弃
*/
int pop1(void);

/*
**返回栈顶的值但不对栈进行修改
*/
STACK_TYPE1 Top1(void);

/*
** 判断是否所空栈
*/
int is_empty1(void);
/*
** 判断是否满
*/
int full1(void);

/*
void create_stack(size_t size);

void destroy_stack(void);
*/
#endif
