#!/bin/bash
#-----------------------------------------------------------------------------
# File Name    : simulate.sh
# Description  : Runs the Processing simulation of the Optiq laser cutter
# Author       : Liam Lawrence
# Created      : May 13, 2018
#
# Version      : 1.0
# Last Updated : n/a
#-----------------------------------------------------------------------------

SKETCHFOLDER="../../processing/simulation"
BUILDFOLDER="$SKETCHFOLDER/build"
processing-java --sketch=$SKETCHFOLDER --output=$BUILDFOLDER --force --run argu "$1" "$2" 
