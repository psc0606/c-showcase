#!/usr/bin/awk -f
#This is an awk script example
# run this script use:
# awk -f example.awk /etc/password
# or
# ./example.awk /etc/password
# author: peng.shaocheng
# date: 2017-07-14
BEGIN {
	FS=":"
}
{print $1}
