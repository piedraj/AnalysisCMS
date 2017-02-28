#!/bin/bash

echo "Creating datacards for " $1
./mkShapesFromHistos.py --pycfg=configuration.py --signalPoint=$1
./mkDatacards.py        --pycfg=configuration.py --inputFile=./Shapes/Shapes$1.root
mkdir -p Datacards/MassPoint$1
rm -r Datacards/MassPoint$1/*
mv tempDatacards/*/ Datacards/MassPoint$1/


