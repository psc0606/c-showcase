#include <stdio.h>
 int main()
 {
 	union temp
 	{
 		char a;
 		short b;
 	}t;
 	t.b = 266;
 	t.a = 66;
 	
 	printf("%x,%d,%x %d\n",&t.a,t.a,&t.b,t.b);
 }
