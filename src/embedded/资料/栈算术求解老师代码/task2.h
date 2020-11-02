#ifndef TASK2_H

#define TASK2_H
/*
**栈的模块接口
*/
typedef char STACK_TYPE2; // 栈所存储的值的类型

#define STACK_SIZE 50

STACK_TYPE2 stack2[STACK_SIZE];
static int top2 = -1;

/*
** value压入栈
*/
int push2(STACK_TYPE2 value);

/*
**出栈  并将其丢弃
*/
int pop2(void);

/*
**返回栈顶的值但不对栈进行修改
*/
STACK_TYPE2 Top2(void);

/*
** 判断是否所空栈
*/
int is_empty2(void);
/*
** 判断是否满
*/
int full2(void);

/*
void create_stack(size_t size);

void destroy_stack(void);
*/
#endif
