#!/bin/bash

FILE="../res/step.txt"

printf %s "$(< $FILE)" > $FILE
