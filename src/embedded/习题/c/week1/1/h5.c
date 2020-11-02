/*----------------------------------------
**	试图将第一题改为输入任意行数并打印。
**	打印*号
**----------------------------------------
*/

#include <stdio.h>
void main()
{
	int i;
	char A[100],passwd[10],str[10];
	printf("请输入需要加密的内容:\n");
	gets(A);
	printf("请输入密码:");
	gets(passwd);
	do
		{
			printf("请输入解密密码:");
			gets(str);
		}
	while(strcmp(passwd,str));
	for(i = 0;A[i] != '\0';i++)
		{
			printf("%c",A[i]);
		}
}
