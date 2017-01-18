#!/bin/bash
#This script clears the terminal, display a greeting and gives information
#about currently connected users. The two example variables are set and displayed.

clear	# clear terminal window

echo "The script starts now."

echo "Hi, $USER!"
echo

echo "I will now fetch you a list of connected users:"
echo
set -x	# activate debugging from here
w	# show who is logged on and
set +x	# stop debugging from here
echo	# what they are doing

echo "I am setting two variables now."
COLOR="back"	# set a local shell variable
VALUE="9"	# set a local shell variable
echo "This is a string : $COLOR"
echo "And this is a number:$VALUE"
echo

echo "I'm giving you back your prompt now.'"
echo
