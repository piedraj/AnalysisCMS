#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "  "
    echo "  ./merge-metfilters.sh ../rootfiles/TTDM"
    echo "  "
    exit -1
fi

FOLDER="$1"

pushd $FOLDER

hadd -f -k Run2015D_16Dec2015_DoubleEG.root     Run2015D_16Dec2015_DoubleEG_000*.root
hadd -f -k Run2015D_16Dec2015_DoubleMuon.root   Run2015D_16Dec2015_DoubleMuon_000*.root
hadd -f -k Run2015D_16Dec2015_MuonEG.root       Run2015D_16Dec2015_MuonEG_000*.root
hadd -f -k Run2015D_16Dec2015_MET.root          Run2015D_16Dec2015_MET_000*.root
hadd -f -k Run2015D_16Dec2015_SinglePhoton.root Run2015D_16Dec2015_SinglePhoton_000*.root

popd
