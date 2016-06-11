#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "  "
    echo "  ./Smerge.sh ../rootfiles/<systematic>/<analysis>"
    echo "  ./Smerge.sh ../minitrees/<systematic>/<analysis>"
    echo "  "
    exit -1
fi

FOLDER="$1"

pushd $FOLDER

hadd -f -k 00_Fakes.root     DD_Run*.root
hadd -f -k 01_Data.root      Run*.root
hadd -f -k 02_WZTo3LNu.root  WZTo3LNu.root
hadd -f -k 03_ZZ.root        ZZTo4L.root ZZTo2L2Nu.root ZZTo2L2Q*.root
hadd -f -k 04_TTTo2L2Nu.root TTTo2L2Nu.root
hadd -f -k 05_ST.root        ST_tW_antitop.root ST_tW_top.root
hadd -f -k 06_WW.root        WWTo2L2Nu.root GluGluWWTo2L2Nu_MCFM.root
hadd -f -k 07_ZJets.root     DYJetsToLL_M-10to50*.root DYJetsToLL_M-50*.root
hadd -f -k 08_WJets.root     WJetsToLNu*.root
hadd -f -k 09_TTV.root       TTWJetsToLNu.root
hadd -f -k 10_HWW.root       GluGluHToWWTo2L2Nu_M125.root VBFHToWWTo2L2Nu_M125.root ttHJetToNonbb_M125.root
hadd -f -k 11_Wg.root        Wg_AMCNLOFXFX.root
hadd -f -k 12_Zg.root        Zg.root
hadd -f -k 13_VVV.root       WZZ.root
hadd -f -k 14_HZ.root        HZJ_HToWW_M125.root ggZH_HToWW_M125.root
hadd -f -k 15_WgStar.root    WgStarLNuEE.root WgStarLNuMuMu.root
hadd -f -k 16_HW.root        HWplusJ_HToWW_M125.root HWminusJ_HToWW_M125.root

popd
