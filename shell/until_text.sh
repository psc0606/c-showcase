#! /bin/bash

x=0;
tput rc
#tput ed
until [ $x -eq 9 ];
do 
	let x++;

	tput rc
	tput ed
	echo $x;
	sleep 1; #睡眠1s
done
