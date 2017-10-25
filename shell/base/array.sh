#! /bin/bash
array_var1=(1 2 3 4 5 6)

array_var2[0]="test1"
array_var2[1]="test2"
array_var2[2]="test3"
array_var2[3]="test4"
array_var2[4]="test5"

echo ${array_var1[0]}
echo ${array_var1[*]}
echo ${#array_var1[*]}

echo ${array_var2[0]}
echo ${array_var2[*]}
echo ${#array_var2[*]}
