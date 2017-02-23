#!/bin/bash

echo "Creating datacards for the validation regions"
./mkShapesFromHistos.py --pycfg=configurationVR.py --outputFileName=ShapesVR1
./mkDatacards.py        --pycfg=configurationVR.py --inputFile=./Shapes/ShapesVR1.root

mkdir -p Datacards/ValidationRegions
rm -r Datacards/ValidationRegions/*
mv tempDatacards/*/ Datacards/ValidationRegions/


