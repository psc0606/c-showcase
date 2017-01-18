#!/bin/sh
#!/bin/sh
DIR=`pwd`
WANT=`echo $DIR | sed "s/\// /g"`
t=0
for i in $WANT
do
	t=`expr "$t" "+" 1`
done
t=`expr "$t" "+" 1`
WANT=`echo $DIR | cut -d/ -f$t-`
echo $WANT

