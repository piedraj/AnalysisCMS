#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "  "
    echo "  ./merge.sh ../rootfiles/25ns"
    echo "  "
    exit -1
fi

FOLDER="$1"

pushd $FOLDER

hadd -f -k 01_Data.root      Run2015D*.root
hadd -f -k 02_WZ.root        WZTo3LNu.root
hadd -f -k 03_ZZ.root        ZZTo4L__part*.root ggZZ*.root
hadd -f -k 04_Top.root       TT__part*.root
hadd -f -k 05_SingleTop.root ST_s-channel.root ST_t-channel*.root ST_tW_*.root
hadd -f -k 06_WW.root        WWTo2L2Nu.root
hadd -f -k 07_ZJets.root     DYJetsToLL_M*.root
hadd -f -k 08_WJets.root     WJetsToLNu__part*.root
hadd -f -k 09_TTW.root       TTWJetsToLNu.root
hadd -f -k 10_TTZ.root       TTZToLLNuNu_M-10.root
hadd -f -k 11_HWW.root       GluGluHToWWTo2L2Nu_M125.root
hadd -f -k 12_Wg.root        Wg.root
hadd -f -k 13_WWZ.root       WWZ.root
hadd -f -k 14_WZZ.root       WZZ.root

popd

