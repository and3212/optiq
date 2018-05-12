#!/bin/bash

TEXTFILE="../cpp/res/step.txt"
CSVFILE="../cpp/res/gcode.csv"

sed 's/ \+/,/g' $TEXTFILE > $CSVFILE
