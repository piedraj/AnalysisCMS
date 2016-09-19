#!/bin/bash

if [ $# -lt 2 ]; then
    echo "  "
    echo "  ln -s /afs/cern.ch/user/p/piedra/work/CMSSW_projects/CMSSW_8_0_5/src/PlotsConfigurations/Configurations/ControlRegions/WW/samples.py"
    echo "  "
    echo "  ./get-mc-samples.sh \\"
    echo "  samples.py \\"
    echo "  root://eoscms.cern.ch//eos/cms/store/group/phys_higgs/cmshww/amassiro/HWW12fb_v2/07Jun2016_spring16_mAODv2_12pXfbm1/MCl2loose__hadd__bSFL2pTEff__l2tight \\"
    echo "  > samples_mc_l2tight.txt"
    echo "  "
    exit -1
fi

export SAMPLES=$1
export DIRECTORY=$2

cat $SAMPLES | grep latino | grep -v "#" | grep -v "Run201" | awk 'BEGIN { FS = ".root" } ; { print $1 }' | awk -v dir=$DIRECTORY 'BEGIN { FS = "latino_" } ; { print dir"/latino_"$2".root" }'
