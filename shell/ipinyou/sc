#!/bin/bash
#功能: search and copy
#用法: sc [src_dir] [pattern] [des_dir]
#bug : 无法查找目录名包含pattern的路径
#版本: 1.0 2016-6-29 Joe.P

src_dir="$1"
pattern="$2"
des_dir=$3
if [ -z $src_dir ] || [ -z $pattern ] || [ -z $des_dir ]; then
	echo "用法：sc [src_dir] [pattern] [des_dir]"
	exit
fi

#不区分大小写搜索包含指定pattern的文件
res=`grep -irl $directory -e $pattern`

#把所有文件拷贝到目标目录中
for x in $res
do
	echo "从$x 拷贝至 $des_dir$x"
	cp -r $x $des_dir$x
done
