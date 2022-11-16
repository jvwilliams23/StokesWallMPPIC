#!/bin/bash

foamListTimes -noZero -rm

MPPICFoam > log.MPPICFoam

python3 scripts/calcEc.py >> coeffRest.dat

