#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "  "
    echo "  ./merge.sh ../rootfiles/<analysis>"
    echo "  "
    exit -1
fi

FOLDER="$1"

pushd $FOLDER

hadd -f -k 00_Fakes.root    DD_Run*.root
hadd -f -k 01_Data.root     Run*.root
hadd -f -k 02_WZTo3LNu.root WZTo3LNu.root
hadd -f -k 03_ZZ.root       ZZTo4L.root ZZTo2L2Q_000*.root
hadd -f -k 04_Top.root      TTTo2L2Nu.root ST_tW_antitop.root ST_tW_top.root
hadd -f -k 05_WW.root       WWTo2L2Nu.root GluGluWWTo2L2Nu_MCFM.root
hadd -f -k 06_ZJets.root    DYJetsToLL_M-10to50.root DYJetsToLL_M-50_000*.root
hadd -f -k 07_WJets.root    WJetsToLNu.root
hadd -f -k 08_TTV.root      TTWJetsToLNu.root
hadd -f -k 09_HWW.root      GluGluHToWWTo2L2Nu_M124.root VBFHToWWTo2L2Nu_M124.root HZJ_HToWW_M125.root ttHJetToNonbb_M125.root
hadd -f -k 10_Wg.root       Wg500.root
hadd -f -k 11_Zg.root       Zg.root
hadd -f -k 12_VVV.root      WZZ.root

popd
