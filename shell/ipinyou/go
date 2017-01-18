#!/bin/bash
#功能：使用gnome文件管理器打开当前命令所在的路径
#范围：适用gnome桌面
#时间：2016-6-24 Joe.P

#nautius是gnome文件夹管理器
path=$1
if [ -z "$path" ]; then
	nautilus `pwd` &> /dev/null &
	exit
else
	nautilus $path &> /dev/null &
fi

exit
