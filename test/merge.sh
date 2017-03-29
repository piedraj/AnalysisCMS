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

rm -rf 00_Fakes.root
rm -rf 01_Data.root
rm -rf 02_WZTo3LNu.root
rm -rf 03_VZ.root
rm -rf 04_TTTo2L2Nu.root
rm -rf 05_ST.root
rm -rf 06_WW.root
rm -rf 07_ZJets.root
rm -rf 08_WJets.root
rm -rf 09_TTV.root
rm -rf 10_HWW.root
rm -rf 11_Wg.root
rm -rf 12_Zg.root
rm -rf 13_VVV.root
rm -rf 14_HZ.root
rm -rf 15_WgStar.root


 hadd -f -k 00_Fakes.root     fakeSel*.root
 hadd -f -k 01_Data.root      *Run2016*.root
 hadd -f -k 02_WZTo3LNu.root  WZTo3LNu.root
 hadd -f -k 03_VZ.root        ZZTo2L2Q__part*.root WZTo2L2Q__part*.root
 hadd -f -k 04_TTTo2L2Nu.root TTTo2L2Nu__part*.root
 hadd -f -k 05_ST.root        ST_tW_antitop.root ST_tW_top.root
 hadd -f -k 06_WW.root        WWTo2L2Nu.root #GluGluWWTo2L2Nu_MCFM.root
 hadd -f -k 07_ZJets.root     DYJetsToLL_M-10to50.root DYJetsToLL_M-50*.root
#hadd -f -k 08_WJets.root     WJetsToLNu.root
 hadd -f -k 09_TTV.root       TTWJetsToLNu.root TTWJetsToLNu_ext2.root TTWJetsToQQ.root TTZToQQ.root TTZToLLNuNu_M-10.root 
#hadd -f -k 10_HWW.root       GluGluHToWWTo2L2NuAMCNLO_M125.root VBFHToWWTo2L2Nu_M125.root GluGluHToTauTau_M125.root VBFHToTauTau_M125.root HWminusJ_HToWW_M125.root HWplusJ_HToWW_M125.root
 hadd -f -k 11_Wg.root        Wg_MADGRAPHMLM.root
 hadd -f -k 12_Zg.root        Zg__part*.root
 hadd -f -k 13_VVV.root       WWW.root WWZ.root WZZ.root ZZZ.root
#hadd -f -k 14_HZ.root        HZJ_HToWWTo2L2Nu_M125_noHLT.root GluGluZH_HToWWTo2L2Nu_M125_noHLT.root
 hadd -f -k 15_WgStar.root    WgStarLNuEE.root WgStarLNuMuMu.root

popd
