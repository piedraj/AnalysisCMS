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

hadd -f -k 01_Data_74.root      Run2015D_05Oct2015*.root Run2015D_PromptReco*.root
hadd -f -k 02_Data_76.root      Run2015D_16Dec2015*.root

popd
