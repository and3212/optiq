#!/bin/bash

sed 's/ \+/,/g' ../cpp/res/step.txt > ../cpp/res/gcode.csv
