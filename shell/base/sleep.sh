#! /bin/bash
echo -n Count: #输出字符串Count,并去掉回车
tput sc        #恢复光标原来位置

count=0;
while true;
do
if [ $count -lt 4 ];
then
let count++;
sleep 1;
tput rc
tput ed
echo -n $count;
else echo; exit 0; #echo;只是输出换行符
fi
done
