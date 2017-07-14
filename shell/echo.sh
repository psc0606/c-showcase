#!/bin/bash

#define a variable and value it
a="hello world" #no space between '='

#print
echo "A is $a"

num=2
#actually,won't print 'this is the 2nd2nd'
#just print "this is the "
echo "this is the $numnd"

#print 'this is the 2nd'
echo "this is the ${num}nd"
