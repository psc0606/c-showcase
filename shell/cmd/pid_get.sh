#/bin/bash

#启动端口指定为20000
nohup java -jar -server -Dserver.plain.port=20000 dsp-core-tool-0.0.1-SNAPSHOT.jar > /dev/null &

#指定pid文件名及路径
pid_file="/data/pid.file"

#最后运行的命令的结束代码（返回值）
if [ $? -eq 0 ];then
	echo $! > $pid_file
else
	echo "start up fail!"
	exit 1
fi

#Shell最后运行的后台Process的PID
echo $! > $pid_file
echo  "running pid is `cat $pid_file`"
