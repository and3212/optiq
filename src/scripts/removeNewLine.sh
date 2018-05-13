#!/bin/bash
# Author: Liam Lawrence
# Date: May 13, 2018
# Removes an extra \n off of the step file which can cause crashes
 
FILE="../res/step.txt"
printf %s "$(< $FILE)" > $FILE
