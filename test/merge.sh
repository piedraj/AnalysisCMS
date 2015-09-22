#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "  "
    echo "  ./merge.sh ../rootfiles/50ns"
    echo "  ./merge.sh ../rootfiles/25ns"
    echo "  "
    exit -1
fi


FOLDER="$1"


pushd $FOLDER

# 01 Data
# 02 WZ
# 03 ZZ
# 04 Top
# 05 SingleTop
# 06 WW
# 07 ZJets
# 08 WJets
# 09 QCD
	
hadd -f -k 01_Data.root    \
    SingleElectron.root    \
    SingleMu.root          \
    SingleMuon.root        \
    DoubleEG.root          \
    DoubleMuon.root        \
    DoubleMuonLowMass.root \
    MuonEG.root

hadd -f -k 02_WZ.root \
    WZ.root

hadd -f -k 03_ZZ.root \
    ZZ.root           \
    ZZTo2L2Nu.root

hadd -f -k 04_Top.root \
    TTJets.root

hadd -f -k 05_SingleTop.root \
    ST_t-channel.root        \
    ST_tW_antitop.root       \
    ST_tW_top.root

hadd -f -k 06_WW.root \
    WWTo2L2Nu_NLL.root

hadd -f -k 07_ZJets.root \
    DYJetsToLL_M-50.root

hadd -f -k 08_WJets.root \
    WJetsToLNu.root

hadd -f -k 09_QCD.root \
    QCD_Pt-15to20_MuEnrichedPt5.root

popd
