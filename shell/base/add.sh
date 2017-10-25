#! /bin/bash
no1=4
no2=5;
let res1=no1+no2
res2=$[no1+no2]
res3=$((no1+no2))

echo $res1 $res2 $res3
