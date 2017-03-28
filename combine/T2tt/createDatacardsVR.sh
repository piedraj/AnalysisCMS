#!/bin/bash

echo "Creating datacards for the validation regions"
./mkShapesFromHistos.py --pycfg=configurationVR.py --signalPoint=_mStop-250to350_Sm300_Xm125 --outputFileName=ShapesVR1
./mkDatacards.py        --pycfg=configurationVR.py --inputFile=./Shapes/ShapesVR1_mStop-250to350_Sm300_Xm125.root

mkdir -p Datacards/ValidationRegions
rm -r Datacards/ValidationRegions/*
mv tempDatacards/*/ Datacards/ValidationRegions/


