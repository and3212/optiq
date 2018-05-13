#!/bin/bash
# Author: Liam Lawrence
# Date: May 13, 2018
# Runs the processing simulation with variables passed from the C++ code

SKETCHFOLDER="../../processing/simulation"
BUILDFOLDER="$SKETCHFOLDER/build"
processing-java --sketch=$SKETCHFOLDER --output=$BUILDFOLDER --force --run argu "$1" "$2" 
