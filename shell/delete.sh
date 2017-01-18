# This is shell script to delete taggets in the all directory
#!/bin/bash
targets=tags #tagets will be deleted
file=$(find . -name $targets) #find all directory
#echo $file
rm -f $file
