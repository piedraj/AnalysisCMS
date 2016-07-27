#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  ./get-mc-samples.sh samples.py eos/cms/store/group/phys_higgs/cmshww/amassiro/HWW6p3/07Jun2016_spring16_mAODv2_6p3fbm1/MCl2loose__hadd__bSFL2pTEff__l2tight__wwSel > samples_mc_l2tight.txt"
   echo "  "
    exit -1
fi

export SAMPLES=$1
export DIRECTORY=$2

cat $SAMPLES | grep latino | grep -v "#" | grep -v "Run201" | awk 'BEGIN { FS = ".root" } ; { print $1 }' | awk -v dir=$DIRECTORY 'BEGIN { FS = "latino_" } ; { print dir"/latino_"$2".root" }'
