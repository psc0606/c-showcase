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
}
{}
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
}
