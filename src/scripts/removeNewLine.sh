#!/bin/bash
#-----------------------------------------------------------------------------
# File Name    : removeNewLine.sh
# Description  : Removes an extra \n off of the step file which can sometimes cause a crash
# Author       : Liam Lawrence
# Created      : May 13, 2018
#
# Version      : 1.0
# Last Updated : n/a
#-----------------------------------------------------------------------------

FILE="../res/step.txt"
printf %s "$(< $FILE)" > $FILE
