#!/bin/sh
#protocol buffer协议生成器
#需要在命令行指定.proto文件地址,默认在当前目录下生成java文件
#proto文件必须在当前目录下，同时-I参数必须是具体的proto文件后缀才可以识别，否则不识别,目前不管它
src_file=$1
protoc --java_out=./ -I./ $src_file
