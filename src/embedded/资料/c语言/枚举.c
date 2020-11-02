#include <stdio.h>
int main()
{
	enum season{spring,summer,autumn = 0,winter}s1,s2,s3,s4;
	s1 = summer;
//	s2 = (enum season)2;
	s2 = winter;
	printf("s1= %d ,s2 = %d\n",s1,s2);
}
