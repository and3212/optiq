#!/bin/bash

TEXTFILE="../res/step.txt"
CSVFILE="../res/gcode.csv"

sed 's/ \+/,/g' $TEXTFILE > $CSVFILE
