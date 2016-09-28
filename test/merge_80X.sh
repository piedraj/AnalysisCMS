#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "  "
    echo "  ./merge.sh ../rootfiles/<systematic>/<analysis>"
    echo "  ./merge.sh ../minitrees/<systematic>/<analysis>"
    echo "  "
    exit -1
fi

FOLDER="$1"

pushd $FOLDER

hadd -f -k 00_Fakes.root     DD_Run*.root
hadd -f -k 01_Data.root      Run*.root
hadd -f -k 02_WZTo3LNu.root  WZTo2L2Q__*.root WZTo3LNu.root 
hadd -f -k 03_ZZ.root        ZZTo2L2Q__*.root ZZTo2L2Nu.root
hadd -f -k 04_TTTo2L2Nu.root TTTo2L2Nu_ext1*.root
hadd -f -k 05_ST.root        ST_tW_antitop.root ST_tW_top.root 
hadd -f -k 06_WW.root        WWTo2L2Nu.root GluGluWWTo2L2Nu_MCFM.root
hadd -f -k 07_ZJets.root     DYJetsToLL_M-10to50.root DYJetsToLL_M-50_0000*.root DYJetsToLL_M-50_0001*.root DYJetsToLL_M-50_0002__part0.root
hadd -f -k 08_WJets.root     WJetsToLNu.root
hadd -f -k 09_TTV.root       TTWJetsToLNu.root
hadd -f -k 10_HWW.root       GluGluHToWWTo2L2NuPowheg_M125.root VBFHToWWTo2L2Nu_alternative_M125.root 
#hadd -f -k 11_Wg.root        Wg_MADGRAPHMLM.root
#hadd -f -k 12_Zg.root        Zg.root // does not exits in latinos configuration web
hadd -f -k 13_VVV.root       WZZ.root ZZZ.root WWW.root WWZ.root WWZ.root
hadd -f -k 14_HZ.root        HZJ_HToWW_M125.root ggZH_HToWW_M120.root
##hadd -f -k 15_WgStar.root    WgStarLNuEE.root WgStarLNuMuMu.root

popd
