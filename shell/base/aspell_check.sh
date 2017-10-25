#! /bin/bash

word=$1

# ``反引号, 里面的字符串被shell解释
output=`echo \"$word\" | aspell list`
if [  -z $output ]; then
	echo $word is a dictionary word;
else	
	echo $word is not a dictionary word; 
fi
