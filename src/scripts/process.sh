#!/bin/bash

SKETCHFOLDER="../../processing/simulation"
BUILDFOLDER="$SKETCHFOLDER/build"

processing-java --sketch=$SKETCHFOLDER --output=$BUILDFOLDER --force --run
