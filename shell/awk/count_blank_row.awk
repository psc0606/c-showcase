#!/usr/bin/awk -f
#示例：./count_blank_row.awk address.txt

#统计空白行
BEGIN {
	count = 0;
}
#模式匹配空白行
/^$/ { count++ }
END{ 
	print count;
	count = 1;

	#同C语言中的do while循环
	do {
		print "find a count";
	} while( count != 1);

	while(1) {
		print "while loop"
		break;
	}

	#同C语言中的for循环
	for(var = 1; var < 10; var++) {
		print var;
	}
}
