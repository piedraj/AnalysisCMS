#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "  "
    echo "  ./merge.sh ../rootfiles/<analysis>"
    echo "  "
    exit -1
fi

FOLDER="$1"

pushd $FOLDER

hadd -f -k 01_Data.root  Run2015D*.root Run2915D*.root
hadd -f -k 02_WZ.root    WZTo3LNu.root
hadd -f -k 03_ZZ.root    ZZTo4L.root ggZZ*.root
hadd -f -k 04_Top.root   TTJets.root ST_s-channel.root ST_t-channel.root ST_tW_antitop.root ST_tW_top.root
hadd -f -k 05_WW.root    WWTo2L2Nu.root GluGluWWTo2L2Nu_MCFM.root
hadd -f -k 06_ZJets.root DYJetsToLL_M-10to50.root DYJetsToLL_M-50.root
hadd -f -k 07_WJets.root WJetsToLNu.root
hadd -f -k 08_TTV.root   TTWJetsToLNu.root TTZToLLNuNu_M-10.root
hadd -f -k 09_HWW.root   GluGluHToWWTo2L2Nu_M125.root
hadd -f -k 10_Wg.root    Wg.root
hadd -f -k 11_Zg.root    Zg.root
hadd -f -k 12_VVV.root   WWZ.root WZZ.root ZZZ.root

popd

