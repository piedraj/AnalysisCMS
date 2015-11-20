#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "  "
    echo "  ./merge.sh ../rootfiles/25ns"
    echo "  "
    exit -1
fi

FOLDER="$1"

pushd $FOLDER

if [[ $FOLDER == *"50ns"* ]] ; then

    hadd -f -k 01_Data.root      SingleElectron.root SingleMuon.root DoubleEG.root DoubleMuon.root MuonEG.root
    hadd -f -k 02_WZ.root        WZ.root
    hadd -f -k 03_ZZ.root        ZZ.root
    hadd -f -k 04_Top.root       TTJets.root
    hadd -f -k 05_SingleTop.root ST_t-channel.root ST_tW_antitop.root ST_tW_top.root
    hadd -f -k 06_WW.root        WWTo2L2Nu_NLL.root
    hadd -f -k 07_ZJets.root     DYJetsToLL_M-50.root
    hadd -f -k 08_WJets.root     WJetsToLNu.root

elif [[ $FOLDER == *"25ns"* ]] ; then

    hadd -f -k 01_Data.root      Run2015D*.root
    hadd -f -k 02_WZ.root        WZTo3LNu.root
    hadd -f -k 03_ZZ.root        ZZ.root
    hadd -f -k 04_Top.root       TTTo2L2Nu.root
    hadd -f -k 05_SingleTop.root ST_t-channel.root
    hadd -f -k 06_WW.root        WWTo2L2Nu.root
    hadd -f -k 07_ZJets.root     DYJetsToLL_M*.root
    hadd -f -k 08_WJets.root     WJetsToLNu.root
    hadd -f -k 09_TTW.root       TTWJetsToLNu.root
#   hadd -f -k 10_TTZ.root       TTZToLLNuNu_M-10.root
    hadd -f -k 11_HWW.root       GluGluHToWWTo2L2Nu_M125.root

fi

popd

