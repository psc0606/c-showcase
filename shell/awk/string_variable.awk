#! /user/bin/awk -f

#awk字符串操作，结束分号不是必须的, 加分号可读性更强
BEGIN {
	# 定义字符串
	# 注意字符串不能使用单引号定义
	str1 = "Test1\tTest2";
	
	printf("length:%d\n", length(str1))		#字符串长度
	
	printf("tolower:%s\n", tolower(str1))   #转换成小写

	printf("toupper:%s\n", toupper(str1))   #转换成大写

	printf("position:%d\n", index(str1, "Test2"))  #子串位置

	printf("substr:%s\n", substr(str1, 7, 2)) #子串

	#查找匹配
	if(match(str1, "[a-zA-z]+2")) {
		print "find pattern in str1";
	}

	#gsub返回数值，str1值已经被替换
	printf("substitued string:%d\n", gsub("Test2", "***", str1));
	print str1
	printf("substitued string:%d\n", gsub("^T", "***", str1));
	print str1
}
{}
END{}
