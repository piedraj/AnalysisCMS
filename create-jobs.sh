#!/bin/bash

if [ $# -lt 1 ]; then
    echo "  "
    echo "  ./create-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/Data13TeVRun2015B"
    echo "  ./create-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/50ns_August_PU"
    echo "  ./create-jobs.sh /gpfs/csic_projects/tier3data/LatinosSkims/MC_Spring15/25ns_August_PU"
    echo "  "
    exit -1
fi

export INPUT_PATH=$1

ls -1 $INPUT_PATH | awk '{ sub(/latino_/, ""); print }' | awk '{ sub(/.root/, ""); print "./runWZ \""$1"\" \"'$INPUT_PATH'/latino_"$1".root\"" }'

