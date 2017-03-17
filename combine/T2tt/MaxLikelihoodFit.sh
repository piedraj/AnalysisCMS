#!/bin/bash

combine -M MaxLikelihoodFit --saveShapes Datacards/ValidationRegions/datacard$1.txt
mv mlfit.root Datacards/ValidationRegions/mlfit$1.root
