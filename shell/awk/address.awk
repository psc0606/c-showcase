#!/usr/bin/awk -f

BEGIN {
	#指定输入字段分隔符, 默认空格或者\t
	FS = "\n"
	
	#指定输入文件的记录分隔符，默认为换行符
	RS = " "

    OFS = ", "
}
{ 
	print $1, $2, $3
}
END{}
