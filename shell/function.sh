#! /bin/bash

function fname()
{
	echo $1 $2; #访问函数的参数1和参数2
	echo "$@";  #打印所有的参数
	echo "$*"   #
	return 0;
}

fname a b;          #调用函数
#export -f fname     #导出函数到子进程中去
