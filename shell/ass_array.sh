#! /bin/bash
declare -A ass_array
ass_array=([index1]=a [index2]=b)
ass_array[index3]=c
ass_array[index4]=d
echo ${!ass_array[*]}
echo ${ass_array[*]}
