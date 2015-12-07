#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "  "
    echo "  ./merge.sh ../rootfiles/<analysis>"
    echo "  "
    exit -1
fi

FOLDER="$1"

pushd $FOLDER

hadd -f -k 01_Data.root  Run2015D*.root
hadd -f -k 02_WZ.root    WZTo3LNu.root
hadd -f -k 03_ZZ.root    ZZTo4L__part*.root ggZZ*.root
hadd -f -k 04_Top.root   TT__part*.root ST_s-channel.root ST_t-channel*.root ST_tW_*.root
hadd -f -k 05_WW.root    WWTo2L2Nu.root
hadd -f -k 06_ZJets.root DYJetsToLL_M*.root
hadd -f -k 07_WJets.root WJetsToLNu__part*.root
hadd -f -k 08_TTV.root   TTWJetsToLNu.root TTZToLLNuNu_M-10.root
hadd -f -k 09_HWW.root   GluGluHToWWTo2L2Nu_M125.root
hadd -f -k 10_Vg.root    Wg.root Zg__part*.root
hadd -f -k 11_VVV.root   WWZ.root WZZ.root ZZZ.root

popd

