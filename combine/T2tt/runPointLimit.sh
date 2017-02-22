#!/bin/bash

echo "Computing limit for " $1
combine -M Asymptotic  --run expected -n $1 -d Datacards/MassPoint$1/datacardFinal.txt
mv higgsCombine$1.Asymptotic.mH120.root Datacards/MassPoint$1/datacardFinal.root

