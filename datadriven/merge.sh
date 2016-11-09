#!/bin/bash

if [ $# -lt 1 ] ; then
    echo "  "
    echo "  ./merge.sh ../rootfilesFR/nominal/FR"
    echo "  ./merge.sh ../rootfilesPR/nominal/PR"
    echo "  "
    exit -1
fi

FOLDER="$1"

pushd $FOLDER

hadd -f -k 01_Data.root   Run*.root
#hadd -f -k 07_ZJets.root  DYJetsToLL_M-10to50*.root DYJetsToLL_M-50*.root
hadd -f -k 07_ZJets.root  DYJetsToLL_M-50*.root
hadd -f -k 08_WJets.root  WJetsToLNu*.root

popd
