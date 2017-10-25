#!/bin/bash

# 说明: 批量文件重命名
# 使用方式: 把当前脚本放在目标目录中运行
# 格式:
#	必须提供文件命名的前缀和后缀
#	./batch_rename.sh some_prefix some_suffix
# author: Joe.P 2017-10-25

#文件名前缀，命令行参数提供
name_prefix=$1
name_suffix=$2

i=1
for file in ./*
do
    #避免把脚本自己也重命令
	if [ "$file" == "$0" ];then
		continue
	fi

	#文件名重命令: 前缀+索引+后缀
	file_name=$name_prefix$i$name_suffix
	mv $file $file_name
	echo "move file"$file" to "$file_name

	#shell变量自增
	let i+=1
done
