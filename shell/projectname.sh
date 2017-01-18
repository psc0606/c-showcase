#!/bin/sh
#!/bin/sh

#get current path
DIR=`pwd` 

WANT=`echo $DIR | sed "s/\// /g"`
t=0
for i in $WANT
do
	t=`expr "$t" "+" 1`
done
t=`expr "$t" "+" 1`
WANT=`echo $DIR | cut -d/ -f$t-`
WANT=$WANT".6.2"
echo $WANT

