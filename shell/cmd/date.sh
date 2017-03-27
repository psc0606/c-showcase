#!/bin/bash

#Format the current date to the specified string.
#ahead of the current time 1 miniute.
date -d '-1 min' +%Y%m%d%H%M


#Format to date.
date -d '-1 min' +%Y%m%d
