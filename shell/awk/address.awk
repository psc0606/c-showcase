#!/usr/bin/awk -f

BEGIN {
	#指定输出分隔符, 默认空格
	FS = "\n"
	
	#指定输入文件的记录分隔符，默认为换行符
	RS = " "
}
{ 
	print $1, $2, $3
}
END{}
