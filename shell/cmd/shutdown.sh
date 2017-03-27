#!/bin/bash

pid_file="/data/pid.file"
PID=$(cat $pid_file)

if [ -z "$PID" ];then
	echo "no pid.file,please kill it manually"
	exit 1
fi

# kill 10 times, then force kill
TIMEOUT=-1
count=1;
while [ $TIMEOUT -lt 0 -o $count -le $TIMEOUT ]
do
	kill -0 $PID 2>/dev/null
	if [ $? -eq 0 ]; then
		echo kill $PID time [$count]
		kill $PID
		sleep 1
		((count++))
	else
		break
	fi
done

#test if the process exists.
kill -0 $PID 2>/dev/null

if [ $? -eq 0 ];then
	echo kill -9 $PID
	kill -9 $PID
	sleep $TIMEOUT
	echo "kill $PID force"
else
	echo "kill $PID graceful"
fi

