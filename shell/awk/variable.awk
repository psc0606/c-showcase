#! /usr/bin/awk -f
BEGIN {
	#定义整数
	int1 = 10;
	int2 = -10;
	int3 = 10e2; #10乘10的2次方，e不区分大小写

	octal = "\377"; #八进制以\开头
	hex = "\xff"; # 十六进制以 \x 开头，a,b,c,d,e,f 不区分大小写

	#定义浮点数
	float1 = 10.0;
	float2 = 10.;
	float3 = 0.25;
	float4 = .25;

    #awk数组下标从1开始, 这点不同于C语言
	myarray[1] = "This";
	myarray[2] = 1111;
	myarray[3] = 2222;
	myarray["index"] = "xxxx";
}
{ 
	print "命令行参数个数", ARGC;

    #命令行参数数组ARGV
	print "文件名", FILENAME
	print "当前行的列数", NF

	#当前遍历第几行了
	print "当前处理过的行数", NR
	print "当前文件的行号，文件自己的行号", FNR

	#通过split定义数组
	str1 = "a,b,c,d";
	split(str1, alphabet, ",");
	print "split分割字符串", alphabet[1]

	#空格为字符串连接运算符
	x = "Hello" "Wolrd"
	print "字符串连接", x
}
END {
	print int1;
	print int2;
	print int3;

	print octal;
	print hex;

	print float1;
	print float2;
	print float3;
	print float4;

    #awk借鉴C语言，支持丰富的数学运算符
	int1 += 3;
	print int1;
	int2 *= 3;
	print int2;
	int2++;
	print int2;

	print "数组测试"
    #awk for in循环比较特殊，var拿到的是数组的下标, 而且下标的顺序不是特定的
	delete myarray[2] #删除数组元素
	for(var in myarray) {
		print myarray[var]
	}
}
