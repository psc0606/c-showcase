#! /bin/bash

# 获取文件名，去掉后缀
file_jpg="sample.jpg"
name=${file_jpg%.*}
echo $name

# 获取后缀
name2=${file_jpg#*.}
echo $name2

# 贪婪获取文件名
file=www.google.cn
name3=${file%%.*}
name4=${file##*.}
echo $name3 $name4
